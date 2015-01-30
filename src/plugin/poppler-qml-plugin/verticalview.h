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

#ifndef VERTICALVIEW_H
#define VERTICALVIEW_H

#include <private/qquickitemchangelistener_p.h>
#include <private/qquickflickable_p.h>

class QAbstractItemModel;
class QQmlChangeSet;
class QQmlDelegateModel;


/**
    Note for users of this class

    VerticalView already loads delegates async when appropiate so if
    your delegate uses a Loader you should not enable the asynchronous feature since
    that will need to introduce sizing problems

    With the double async it may happen what while we are scrolling down
    we reach to a point where given the size of the just created delegate with loader not yet loaded (which will be very close to 0)
    we are already "at the end" of the list, but then a few milliseconds later the loader finishes loading and we could
    have kept scrolling. This is specially visible at the end of the list where you realize
    that scrolling ended a bit before the end of the list but the speed of the flicking was good
    to reach the end

    By not having the second async we get a better sizing when the delegate is created and things work better
*/

class VerticalView : public QQuickFlickable, public QQuickItemChangeListener
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(int cacheBuffer READ cacheBuffer WRITE setCacheBuffer NOTIFY cacheBufferChanged)
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int currentPageIndex READ currentPageIndex NOTIFY currentPageIndexChanged)
    Q_PROPERTY(QQuickItem *currentPageItem READ currentPageItem NOTIFY currentPageItemChanged)

public:
    VerticalView();
    ~VerticalView();

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);

    QQmlComponent *delegate() const;
    void setDelegate(QQmlComponent *delegate);

    int cacheBuffer() const;
    void setCacheBuffer(int cacheBuffer);

    qreal spacing() const;
    void setSpacing(qreal spacing);

    int count() const;

    int currentPageIndex() const;
    QQuickItem *currentPageItem() const;

    Q_INVOKABLE void positionAtBeginning();
    Q_INVOKABLE void positionAtIndex(int index);
    Q_INVOKABLE void positionAtEnd();

    Q_INVOKABLE QQuickItem *itemAt(int modelIndex) const;

Q_SIGNALS:
    void modelChanged();
    void delegateChanged();
    void cacheBufferChanged();
    void spacingChanged();
    void countChanged();
    void currentPageIndexChanged();
    void currentPageItemChanged();

protected:
    void componentComplete() override;
    void viewportMoved(Qt::Orientations orient) override;
    qreal minYExtent() const override;
    void itemGeometryChanged(QQuickItem *item, const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void updatePolish() override;

private Q_SLOTS:
    void _q_itemCreated(int modelIndex, QObject *object);
    void _q_heightChanged();
    void _q_modelUpdated(const QQmlChangeSet &changeSet, bool reset);
    void _q_updateCurrentPageIndex();

private:
    class ListItem
    {
        public:
            qreal height() const;

            qreal y() const;
            void setY(qreal newY);

            bool culled() const;
            void setCulled(bool culled);

            QQuickItem *m_item;
    };

    void createDelegateModel();

    void layout();
    void refill();
    bool addVisibleItems(qreal fillFrom, qreal fillTo, bool asynchronous);
    bool removeNonVisibleItems(qreal bufferFrom, qreal bufferTo);
    ListItem *createItem(int modelIndex, bool asynchronous);

    void adjustMinYExtent();
    ListItem *itemAtIndex(int modelIndex) const; // Returns the item at modelIndex if has been created
    void releaseItem(ListItem *item);
    void reallyReleaseItem(ListItem *item);
    void updateWatchedRoles();

    QQmlDelegateModel *m_delegateModel;

    // Index we are waiting because we requested it asynchronously
    int m_asyncRequestedIndex;

    // Used to only give a warning once if the delegate does not return objects
    bool m_delegateValidated;

    // Visible indexes, [0] is m_firstValidIndex, [0+1] is m_firstValidIndex +1, ...
    QList<ListItem *> m_visibleItems;
    int m_firstVisibleIndex;

    int m_currentPageIndex;

    qreal m_minYExtent;

    // If any of the heights has changed
    // or new items have been added/removed
    bool m_contentHeightDirty;

    qreal m_previousContentY;

    bool m_inLayout;

    int m_cacheBuffer;
    qreal m_spacing;

    // Qt 5.0 doesn't like releasing the items just after itemCreated
    // so we delay the releasing until the next updatePolish
    QList<ListItem *> m_itemsToRelease;
};


#endif // VERTICALVIEW_H
