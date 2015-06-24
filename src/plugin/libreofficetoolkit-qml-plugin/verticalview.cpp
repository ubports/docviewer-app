/*
 * Copyright (C) 2013-2015 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Some documentation on how this thing works:
 *
 * A flickable has two very important concepts that define the top and
 * height of the flickable area.
 * The top is returned in minYExtent()
 * The height is set using setContentHeight()
 * By changing those two values we can make the list grow up or down
 * as needed. e.g. if we are in the middle of the list
 * and something that is above the viewport grows, since we do not
 * want to change the viewport because of that we just adjust the
 * minYExtent so that the list grows up.
 *
 * The implementation on the list relies on the delegateModel doing
 * most of the instantiation work. You call createItem() when you
 * need to create an item asking for it async or not. If returns null
 * it means the item will be created async and the model will call the
 * itemCreated slot with the item.
 *
 * updatePolish is the central point of dispatch for the work of the
 * class. It is called by the scene graph just before drawing the class.
 * In it we:
 *  * Make sure all items are positioned correctly
 *  * Add/Remove items if needed
 *  * Update the content height if it was dirty
 *
 * m_visibleItems contains all the items we have created at the moment.
 * Actually not all of them are visible since it includes the ones
 * in the cache area we create asynchronously to help performance.
 * The first item in m_visibleItems has the m_firstVisibleIndex in
 * the model. If you actually want to know what is the first
 * item in the viewport you have to find the first non culled element
 * in m_visibleItems
 *
 * The first item of m_visibleItems is the one that defines the
 * positions of all the rest of items (see updatePolish()) and
 * this is why sometimes we move it even if it's not the item
 * that has triggered the function (i.e. in itemGeometryChanged())
 *
 * m_visibleItems is a list of ListItem. Each ListItem
 * will contain a item and potentially a sectionItem. The sectionItem
 * is only there when the list is using sectionDelegate+sectionProperty
 * and this is the first item of the section. Each ListItem is vertically
 * layouted with the sectionItem first and then the item.
 *
 * Note that minYExtent and height are not always totally accurate, since
 * we don't have the items created we can't guess their heights
 * so we can only guarantee the values are correct when the first/last
 * items of the list are visible, otherwise we just live with good enough
 * values that make the list scrollable
 *
 * There are a few things that are not really implemented or tested properly
 * which we don't use at the moment like changing the model, etc.
 * The known missing features are marked with TODOs along the code.
 */

#include "verticalview.h"

#include <QCoreApplication>
#include <QDebug>
#include <qqmlinfo.h>
#include <qqmlengine.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
#include <private/qqmldelegatemodel_p.h>
#include <private/qqmlglobal_p.h>
#include <private/qquickitem_p.h>
#include <private/qquickanimation_p.h>
#pragma GCC diagnostic pop

qreal VerticalView::ListItem::height() const
{
    return m_item->height();
}

qreal VerticalView::ListItem::y() const
{
    return m_item->y();
}

void VerticalView::ListItem::setY(qreal newY)
{
    m_item->setY(newY);
}

bool VerticalView::ListItem::culled() const
{
    return QQuickItemPrivate::get(m_item)->culled;
}

void VerticalView::ListItem::setCulled(bool culled)
{
    QQuickItemPrivate::get(m_item)->setCulled(culled);
}

VerticalView::VerticalView()
 : m_delegateModel(nullptr)
 , m_asyncRequestedIndex(-1)
 , m_delegateValidated(false)
 , m_firstVisibleIndex(-1)
 , m_currentPageIndex(-1)
 , m_minYExtent(0)
 , m_contentHeightDirty(false)
 , m_previousContentY(0)
 , m_inLayout(false)
 , m_cacheBuffer(0)
 , m_spacing(0)
{
    connect(this, SIGNAL(heightChanged()), this, SLOT(_q_heightChanged()));
    connect(this, SIGNAL(contentYChanged()), this, SLOT(_q_updateCurrentPageIndex()));

    setFlickableDirection(QQuickFlickable::HorizontalAndVerticalFlick);
}

VerticalView::~VerticalView()
{
}

QAbstractItemModel *VerticalView::model() const
{
    return m_delegateModel ? m_delegateModel->model().value<QAbstractItemModel *>() : nullptr;
}

void VerticalView::setModel(QAbstractItemModel *model)
{
    if (model != this->model()) {
        if (!m_delegateModel) {
            createDelegateModel();
        } else {
            disconnect(m_delegateModel, SIGNAL(modelUpdated(QQmlChangeSet,bool)), this, SLOT(_q_modelUpdated(QQmlChangeSet,bool)));
        }
        m_delegateModel->setModel(QVariant::fromValue<QAbstractItemModel *>(model));
        connect(m_delegateModel, SIGNAL(modelUpdated(QQmlChangeSet,bool)), this, SLOT(_q_modelUpdated(QQmlChangeSet,bool)));
        Q_EMIT modelChanged();
        polish();
        // TODO?
//         Q_EMIT contentHeightChanged();
//         Q_EMIT contentYChanged();
    }
}

QQmlComponent *VerticalView::delegate() const
{
    return m_delegateModel ? m_delegateModel->delegate() : nullptr;
}

void VerticalView::setDelegate(QQmlComponent *delegate)
{
    if (delegate != this->delegate()) {
        if (!m_delegateModel) {
            createDelegateModel();
        }

        // Cleanup the existing items
        Q_FOREACH(ListItem *item, m_visibleItems)
            releaseItem(item);
        m_visibleItems.clear();
        m_firstVisibleIndex = -1;
        adjustMinYExtent();
        setContentY(0);

        m_delegateModel->setDelegate(delegate);

        Q_EMIT delegateChanged();
        m_delegateValidated = false;
        m_contentHeightDirty = true;
        polish();
    }
}

int VerticalView::cacheBuffer() const
{
    return m_cacheBuffer;
}

void VerticalView::setCacheBuffer(int cacheBuffer)
{
    if (cacheBuffer < 0) {
        qmlInfo(this) << "Cannot set a negative cache buffer";
        return;
    }

    if (cacheBuffer != m_cacheBuffer) {
        m_cacheBuffer = cacheBuffer;
        Q_EMIT cacheBufferChanged();
        polish();
    }
}

qreal VerticalView::spacing() const
{
    return m_spacing;
}

void VerticalView::setSpacing(qreal spacing)
{
    if (spacing < 0) {
        qmlInfo(this) << "Cannot set a negative spacing";
        return;
    }

    if (spacing != m_spacing) {
        m_spacing = spacing;
        Q_EMIT spacingChanged();
        polish();
    }
}

int VerticalView::count() const
{
    if (m_delegateModel)
        return m_delegateModel->count();
    else
        return 0;
}

int VerticalView::currentPageIndex() const
{
    return m_currentPageIndex;
}

QQuickItem *VerticalView::currentPageItem() const
{
    return itemAt(m_currentPageIndex);
}

void VerticalView::_q_updateCurrentPageIndex()
{
    if (!m_visibleItems.isEmpty()) {
        qreal pos = this->contentY() + (this->height() * 0.5);

        int oldCurrentPageIndex = m_currentPageIndex;
        int i = 0;

        Q_FOREACH(ListItem * item, m_visibleItems) {
            if (item->y() < pos && item->y() + item->height() > pos)
                break;

            i++;
        }

        // If spacing is set, there may be no page on posY position,
        // and the Q_FOREACH loop keep on running until the end.
        if (i != m_visibleItems.length())
            m_currentPageIndex = m_firstVisibleIndex + i;

        if (m_currentPageIndex != oldCurrentPageIndex) {
            Q_EMIT currentPageIndexChanged();
            Q_EMIT currentPageItemChanged();
        }

    }
}

void VerticalView::positionAtBeginning()
{
    if (m_delegateModel->count() <= 0)
        return;

    if (m_firstVisibleIndex != 0) {
        // TODO This could be optimized by trying to reuse the interesection
        // of items that may end up intersecting between the existing
        // m_visibleItems and the items we are creating in the next loop
        Q_FOREACH(ListItem *item, m_visibleItems)
            releaseItem(item);
        m_visibleItems.clear();
        m_firstVisibleIndex = -1;

        // Create the item 0, it will be already correctly positioned at createItem()
        ListItem *item = createItem(0, false);
        // Create the subsequent items
        int modelIndex = 1;
        qreal pos = item->y() + item->height();
        const qreal bufferTo = height() + m_cacheBuffer;
        while (modelIndex < m_delegateModel->count() && pos <= bufferTo) {
            if (!(item = createItem(modelIndex, false)))
                break;
            pos += item->height();
            ++modelIndex;
        }

        m_previousContentY = m_visibleItems.first()->y();
    }
    setContentY(m_visibleItems.first()->y());

    // Force update of currentPageIndex
    _q_updateCurrentPageIndex();
}

void VerticalView::positionAtIndex(int index)
{
    if (m_delegateModel->count() <= 0)
        return;

    if (index < m_firstVisibleIndex || index > m_firstVisibleIndex + m_visibleItems.length() - 1) {
        // TODO This could be optimized by trying to reuse the interesection
        // of items that may end up intersecting between the existing
        // m_visibleItems and the items we are creating in the next loop
        Q_FOREACH(ListItem *item, m_visibleItems)
            releaseItem(item);
        m_visibleItems.clear();
        m_firstVisibleIndex = -1;

        // Create the item with the given index, it will be already correctly positioned at createItem()
        // Other items are created when createdItem() signal is emitted.
        createItem(index, false);

        m_previousContentY = m_visibleItems.first()->y();
    }

    setContentY(itemAt(index)->y());

    // Force update of currentPageIndex
    _q_updateCurrentPageIndex();
}

void VerticalView::positionAtEnd()
{
    if (m_delegateModel->count() <= 0)
        return;

    if (m_firstVisibleIndex != m_delegateModel->count() - 1) {
        // TODO This could be optimized by trying to reuse the interesection
        // of items that may end up intersecting between the existing
        // m_visibleItems and the items we are creating in the next loop
        Q_FOREACH(ListItem *item, m_visibleItems)
            releaseItem(item);
        m_visibleItems.clear();
        m_firstVisibleIndex = -1;

        // Create the item 0, it will be already correctly positioned at createItem()
        ListItem *item = createItem(m_delegateModel->count() - 1, false);
        // Create the prior items
        int modelIndex = m_delegateModel->count() - 2;
        qreal pos = item->y() + item->height();
        const qreal bufferFrom = contentY() - m_cacheBuffer;
        while (modelIndex > -1 && pos >= bufferFrom) {
            if (!(item = createItem(modelIndex, false)))
                break;
            pos += item->height();
            --modelIndex;
        }

        m_previousContentY = m_visibleItems.first()->y();
    }
    setContentY(m_visibleItems.first()->y());

    // Force update of currentPageIndex
    _q_updateCurrentPageIndex();
}

static inline bool uFuzzyCompare(qreal r1, qreal r2)
{
    return qFuzzyCompare(r1, r2) || (qFuzzyIsNull(r1) && qFuzzyIsNull(r2));
}

QQuickItem *VerticalView::itemAt(int modelIndex) const
{
    ListItem *item = itemAtIndex(modelIndex);
    if (item)
        return item->m_item;
    else
        return nullptr;
}

qreal VerticalView::minYExtent() const
{
    return m_minYExtent;
}

void VerticalView::componentComplete()
{
    if (m_delegateModel)
        m_delegateModel->componentComplete();

    QQuickFlickable::componentComplete();

    polish();
}

void VerticalView::viewportMoved(Qt::Orientations orient)
{
    // Check we are not being taken down and don't paint anything
    // TODO Check if we still need this in 5.2
    // For reproduction just inifnite loop testDash or testDashContent
    if (!QQmlEngine::contextForObject(this)->parentContext())
        return;

    QQuickFlickable::viewportMoved(orient);
    m_previousContentY = contentY();
    layout();
    polish();
}

void VerticalView::createDelegateModel()
{
    m_delegateModel = new QQmlDelegateModel(qmlContext(this), this);
    connect(m_delegateModel, SIGNAL(createdItem(int,QObject*)), this, SLOT(_q_itemCreated(int,QObject*)));
    connect(m_delegateModel, SIGNAL(countChanged()), this, SIGNAL(countChanged()));
    if (isComponentComplete())
        m_delegateModel->componentComplete();
    updateWatchedRoles();
}

void VerticalView::refill()
{
    if (m_inLayout) {
        return;
    }
    if (!isComponentComplete()) {
        return;
    }

    const qreal from = contentY();
    const qreal to = from + height();
    const qreal bufferFrom = from - m_cacheBuffer;
    const qreal bufferTo = to + m_cacheBuffer;

    bool added = addVisibleItems(from, to, false);
    bool removed = removeNonVisibleItems(bufferFrom, bufferTo);
    added |= addVisibleItems(bufferFrom, bufferTo, true);

    if (added || removed) {
        m_contentHeightDirty = true;
    }
}

bool VerticalView::addVisibleItems(qreal fillFrom, qreal fillTo, bool asynchronous)
{
    if (!delegate())
        return false;

    if (m_delegateModel->count() == 0)
        return false;

    ListItem *item;
    int modelIndex = 0;
    qreal pos = 0;
    if (!m_visibleItems.isEmpty()) {
        modelIndex = m_firstVisibleIndex + m_visibleItems.count();
        item = m_visibleItems.last();
        pos = item->y() + item->height() + m_spacing;
    }
    bool changed = false;

    while (modelIndex < m_delegateModel->count() && pos <= fillTo) {
        if (!(item = createItem(modelIndex, asynchronous)))
            break;
        pos += item->height() + m_spacing;
        ++modelIndex;
        changed = true;
    }

    modelIndex = 0;
    pos = 0;
    if (!m_visibleItems.isEmpty()) {
        modelIndex = m_firstVisibleIndex - 1;
        item = m_visibleItems.first();
        pos = item->y();
    }
    while (modelIndex >= 0 && pos > fillFrom) {
        if (!(item = createItem(modelIndex, asynchronous)))
            break;
        pos -= item->height() + m_spacing;
        --modelIndex;
        changed = true;
    }

    return changed;
}

void VerticalView::reallyReleaseItem(ListItem *listItem)
{
    QQuickItem *item = listItem->m_item;
    QQmlDelegateModel::ReleaseFlags flags = m_delegateModel->release(item);
    if (flags & QQmlDelegateModel::Destroyed) {
        item->setParentItem(nullptr);
    }
    delete listItem;
}

void VerticalView::releaseItem(ListItem *listItem)
{
    QQuickItemPrivate *itemPrivate = QQuickItemPrivate::get(listItem->m_item);
    itemPrivate->removeItemChangeListener(this, QQuickItemPrivate::Geometry);
    m_itemsToRelease << listItem;
}

void VerticalView::updateWatchedRoles()
{
    if (m_delegateModel) {
        QList<QByteArray> roles;
        m_delegateModel->setWatchedRoles(roles);
    }
}

bool VerticalView::removeNonVisibleItems(qreal bufferFrom, qreal bufferTo)
{
    // Do not remove items if we are overshooting up or down, since we'll come back
    // to the "stable" position and delete/create items without any reason
    if (contentY() < -m_minYExtent) {
        return false;
    } else if (contentY() + height() > contentHeight()) {
        return false;
    }
    bool changed = false;

    bool foundVisible = false;
    int i = 0;
    int removedItems = 0;
    const auto oldFirstVisibleIndex = m_firstVisibleIndex;
    while (i < m_visibleItems.count()) {
        ListItem *item = m_visibleItems[i];
        const qreal pos = item->y() + m_spacing;
        if (pos + item->height() < bufferFrom || pos > bufferTo) {
            releaseItem(item);
            m_visibleItems.removeAt(i);
            changed = true;
            ++removedItems;
        } else {
            if (!foundVisible) {
                foundVisible = true;
                const int itemIndex = m_firstVisibleIndex + removedItems + i;
                m_firstVisibleIndex = itemIndex;
            }
            ++i;
        }
    }
    if (!foundVisible) {
        m_firstVisibleIndex = -1;
    }
    if (m_firstVisibleIndex != oldFirstVisibleIndex) {
        adjustMinYExtent();
    }

    return changed;
}

VerticalView::ListItem *VerticalView::createItem(int modelIndex, bool asynchronous)
{
    if (asynchronous && m_asyncRequestedIndex != -1)
        return nullptr;

    m_asyncRequestedIndex = -1;
    QObject* object = m_delegateModel->object(modelIndex, asynchronous);
    QQuickItem *item = qmlobject_cast<QQuickItem*>(object);
    if (!item) {
        if (object) {
            m_delegateModel->release(object);
            if (!m_delegateValidated) {
                m_delegateValidated = true;
                QObject* delegateObj = delegate();
                qmlInfo(delegateObj ? delegateObj : this) << "Delegate must be of Item type";
            }
        } else {
            m_asyncRequestedIndex = modelIndex;
        }
        return 0;
    } else {
        ListItem *listItem = new ListItem;
        listItem->m_item = item;
        QQuickItemPrivate::get(item)->addItemChangeListener(this, QQuickItemPrivate::Geometry);
        ListItem *prevItem = itemAtIndex(modelIndex - 1);
        bool lostItem = false; // Is an item that we requested async but because of model changes
                               // it is no longer attached to any of the existing items (has no prev nor next item)
                               // nor is the first item
        if (prevItem) {
            listItem->setY(prevItem->y() + prevItem->height() + m_spacing);
        } else {
            ListItem *currItem = itemAtIndex(modelIndex);
            if (currItem) {
                // There's something already in m_visibleItems at out index, meaning this is an insert, so attach to its top
                listItem->setY(currItem->y() - listItem->height() - m_spacing);
            } else {
                ListItem *nextItem = itemAtIndex(modelIndex + 1);
                if (nextItem) {
                    listItem->setY(nextItem->y() - listItem->height() - m_spacing);
                } else if (modelIndex == 0) {
                    listItem->setY(0);
                } else if (!m_visibleItems.isEmpty()) {
                    lostItem = true;
                }
            }
        }
        if (lostItem) {
            listItem->setCulled(true);
            releaseItem(listItem);
            listItem = nullptr;
        } else {
            listItem->setCulled(listItem->y() + listItem->height() + m_spacing <= contentY() || listItem->y() >= contentY() + height());
            if (m_visibleItems.isEmpty()) {
                m_visibleItems << listItem;
            } else {
                m_visibleItems.insert(modelIndex - m_firstVisibleIndex, listItem);
            }
            if (m_firstVisibleIndex < 0 || modelIndex < m_firstVisibleIndex) {
                m_firstVisibleIndex = modelIndex;
                polish();
            }
            adjustMinYExtent();
            m_contentHeightDirty = true;
        }
        return listItem;
    }
}

void VerticalView::_q_itemCreated(int modelIndex, QObject *object)
{
    QQuickItem *item = qmlobject_cast<QQuickItem*>(object);
    if (!item) {
        qWarning() << "VerticalView::itemCreated got a non item for index" << modelIndex;
        return;
    }

    // Check we are not being taken down and don't paint anything
    // TODO Check if we still need this in 5.2
    // For reproduction just inifnite loop testDash or testDashContent
    if (!QQmlEngine::contextForObject(this)->parentContext())
        return;

    item->setParentItem(contentItem());
    QQmlContext *context = QQmlEngine::contextForObject(item)->parentContext();
    context->setContextProperty(QLatin1String("VerticalView"), this);
    context->setContextProperty(QLatin1String("heightToClip"), QVariant::fromValue<int>(0));
    if (modelIndex == m_asyncRequestedIndex) {
        createItem(modelIndex, false);
        refill();
    }
}

void VerticalView::_q_heightChanged()
{
    polish();
}

void VerticalView::_q_modelUpdated(const QQmlChangeSet &changeSet, bool /*reset*/)
{
    // TODO Do something with reset
    const auto oldFirstVisibleIndex = m_firstVisibleIndex;

    Q_FOREACH(const QQmlChangeSet::Change &remove, changeSet.removes()) {
        if (remove.index + remove.count > m_firstVisibleIndex && remove.index < m_firstVisibleIndex + m_visibleItems.count()) {
            const qreal oldFirstValidIndexPos = m_visibleItems.first()->y();
            // If all the items we are removing are either not created or culled
            // we have to grow down to avoid viewport changing
            bool growDown = true;
            for (int i = 0; growDown && i < remove.count; ++i) {
                const int modelIndex = remove.index + i;
                ListItem *item = itemAtIndex(modelIndex);
                if (item && !item->culled()) {
                    growDown = false;
                }
            }
            for (int i = remove.count - 1; i >= 0; --i) {
                const int visibleIndex = remove.index + i - m_firstVisibleIndex;
                if (visibleIndex >= 0 && visibleIndex < m_visibleItems.count()) {
                    ListItem *item = m_visibleItems[visibleIndex];
                    releaseItem(item);
                    m_visibleItems.removeAt(visibleIndex);
                }
            }
            if (growDown) {
                adjustMinYExtent();
            } else if (remove.index <= m_firstVisibleIndex) {
                if (!m_visibleItems.isEmpty()) {
                    // We removed the first item that is the one that positions the rest
                    // position the new first item correctly
                    m_visibleItems.first()->setY(oldFirstValidIndexPos);
                } else {
                    m_firstVisibleIndex = -1;
                }
            }
        } else if (remove.index + remove.count <= m_firstVisibleIndex) {
            m_firstVisibleIndex -= remove.count;
        }
        for (int i = remove.count - 1; i >= 0; --i) {
            const int modelIndex = remove.index + i;
            if (modelIndex == m_asyncRequestedIndex) {
                m_asyncRequestedIndex = -1;
            } else if (modelIndex < m_asyncRequestedIndex) {
                m_asyncRequestedIndex--;
            }
        }
    }

    Q_FOREACH(const QQmlChangeSet::Change &insert, changeSet.inserts()) {
        const bool insertingInValidIndexes = insert.index > m_firstVisibleIndex && insert.index < m_firstVisibleIndex + m_visibleItems.count();
        const bool firstItemWithViewOnTop = insert.index == 0 && m_firstVisibleIndex == 0 && m_visibleItems.first()->y()  > contentY();
        if (insertingInValidIndexes || firstItemWithViewOnTop)
        {
            // If the items we are adding won't be really visible
            // we grow up instead of down to not change the viewport
            bool growUp = false;
            if (!firstItemWithViewOnTop) {
                for (int i = 0; i < m_visibleItems.count(); ++i) {
                    if (!m_visibleItems[i]->culled()) {
                        if (insert.index <= m_firstVisibleIndex + i) {
                            growUp = true;
                        }
                        break;
                    }
                }
            }

            const qreal oldFirstValidIndexPos = m_visibleItems.first()->y();
            for (int i = insert.count - 1; i >= 0; --i) {
                const int modelIndex = insert.index + i;
                ListItem *item = createItem(modelIndex, false);
                if (growUp) {
                    ListItem *firstItem = m_visibleItems.first();
                    firstItem->setY(firstItem->y() - item->height());
                }
            }
            if (firstItemWithViewOnTop) {
                ListItem *firstItem = m_visibleItems.first();
                firstItem->setY(oldFirstValidIndexPos);
            }
            adjustMinYExtent();
        } else if (insert.index <= m_firstVisibleIndex) {
            m_firstVisibleIndex += insert.count;
        }

        for (int i = insert.count - 1; i >= 0; --i) {
            const int modelIndex = insert.index + i;
            if (modelIndex <= m_asyncRequestedIndex) {
                m_asyncRequestedIndex++;
            }
        }
    }

    if (m_firstVisibleIndex != oldFirstVisibleIndex) {
        adjustMinYExtent();
    }

    layout();
    polish();
    m_contentHeightDirty = true;
}

void VerticalView::itemGeometryChanged(QQuickItem * /*item*/, const QRectF &newGeometry, const QRectF &oldGeometry)
{
    const qreal heightDiff = newGeometry.height() - oldGeometry.height();
    if (heightDiff != 0) {
        if (oldGeometry.y() + oldGeometry.height() <= contentY() && !m_visibleItems.isEmpty()) {
            ListItem *firstItem = m_visibleItems.first();
            firstItem->setY(firstItem->y() - heightDiff);
            adjustMinYExtent();
            layout();
        }
        refill();
        adjustMinYExtent();
        polish();
        m_contentHeightDirty = true;
    }
}

void VerticalView::adjustMinYExtent()
{
    if (m_visibleItems.isEmpty()) {
        m_minYExtent = 0;
    } else {
        qreal nonCreatedHeight = 0;
        if (m_firstVisibleIndex != 0) {
            // Calculate the average height of items to estimate the position of the list start
            const int visibleItems = m_visibleItems.count();
            qreal visibleItemsHeight = 0;
            Q_FOREACH(ListItem *item, m_visibleItems) {
                visibleItemsHeight += item->height() + m_spacing;
            }
            nonCreatedHeight = m_firstVisibleIndex * visibleItemsHeight / visibleItems;
        }
        m_minYExtent = nonCreatedHeight - m_visibleItems.first()->y();
        if (m_minYExtent != 0 && qFuzzyIsNull(m_minYExtent)) {
            m_minYExtent = 0;
            m_visibleItems.first()->setY(nonCreatedHeight);
        }
    }
}

VerticalView::ListItem *VerticalView::itemAtIndex(int modelIndex) const
{
    const int visibleIndexedModelIndex = modelIndex - m_firstVisibleIndex;
    if (visibleIndexedModelIndex >= 0 && visibleIndexedModelIndex < m_visibleItems.count())
        return m_visibleItems[visibleIndexedModelIndex];

    return nullptr;
}

void VerticalView::layout()
{
    if (m_inLayout)
        return;

    m_inLayout = true;
    if (!m_visibleItems.isEmpty()) {
        const qreal visibleFrom = contentY();
        const qreal visibleTo = contentY() + height();

        qreal pos = m_visibleItems.first()->y();

//         qDebug() << "VerticalView::layout Updating positions and heights. contentY" << contentY() << "minYExtent" << minYExtent();
        int firstReallyVisibleItem = -1;
        int modelIndex = m_firstVisibleIndex;
        Q_FOREACH(ListItem *item, m_visibleItems) {
            const bool cull = pos + item->height() + m_spacing <= visibleFrom || pos >= visibleTo;
            item->setCulled(cull);
            item->setY(pos);
            if (!cull && firstReallyVisibleItem == -1) {
                firstReallyVisibleItem = modelIndex;
            }
            QQmlContext *context = QQmlEngine::contextForObject(item->m_item)->parentContext();
            const qreal clipFrom = visibleFrom;
            if (!cull && pos < clipFrom) {
                context->setContextProperty(QLatin1String("heightToClip"), clipFrom - pos);
            } else {
                context->setContextProperty(QLatin1String("heightToClip"), QVariant::fromValue<int>(0));
            }
//             qDebug() << "VerticalView::layout" << item->m_item;
            pos += item->height() + m_spacing;
            ++modelIndex;
        }
    }
    m_inLayout = false;
}

void VerticalView::updatePolish()
{
    // Check we are not being taken down and don't paint anything
    // TODO Check if we still need this in 5.2
    // For reproduction just inifnite loop testDash or testDashContent
    if (!QQmlEngine::contextForObject(this)->parentContext())
        return;

    Q_FOREACH(ListItem *item, m_itemsToRelease)
        reallyReleaseItem(item);
    m_itemsToRelease.clear();

    if (!model())
        return;

    layout();

    refill();

    if (m_contentHeightDirty) {
        qreal contentHeight;
        if (m_visibleItems.isEmpty()) {
            contentHeight = 0;
        } else {
            const int modelCount = model()->rowCount();
            const int visibleItems = m_visibleItems.count();
            const int lastValidIndex = m_firstVisibleIndex + visibleItems - 1;
            qreal nonCreatedHeight = 0;
            if (lastValidIndex != modelCount - 1) {
                const int visibleItems = m_visibleItems.count();
                qreal visibleItemsHeight = 0;
                Q_FOREACH(ListItem *item, m_visibleItems) {
                    visibleItemsHeight += item->height() + m_spacing;
                }
                const int unknownSizes = modelCount - (m_firstVisibleIndex + visibleItems);
                nonCreatedHeight = unknownSizes * visibleItemsHeight / visibleItems;
            }
            ListItem *item = m_visibleItems.last();
            contentHeight = nonCreatedHeight + item->y() + item->height();
            if (m_firstVisibleIndex != 0) {
                // Make sure that if we are shrinking we tell the view we still fit
                m_minYExtent = qMax(m_minYExtent, -(contentHeight - height()));
            }
        }

        m_contentHeightDirty = false;
        adjustMinYExtent();
        setContentHeight(contentHeight);
    }
}

#include "moc_verticalview.cpp"
