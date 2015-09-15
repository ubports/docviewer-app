/*
 * Copyright (C) 2015 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "lopartsmodel.h"
#include "lodocument.h"
#include "lopartsimageprovider.h"

#include <QQmlContext>
#include <QQmlEngine>
#include <QDebug>

LOPartsModel::LOPartsModel(QAbstractListModel *parent):
    QAbstractListModel(parent)
{   
    connect(this, SIGNAL(documentChanged()), this, SLOT(fillModel()));
}

void LOPartsModel::setDocument(LODocument *document)
{
    if (m_document == document)
        return;

    m_document = document;
    Q_EMIT documentChanged();

    QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
    QString imageProviderName = "lok";

    if (engine->imageProvider(imageProviderName))
        engine->removeImageProvider(imageProviderName);

    engine->addImageProvider(imageProviderName, new LOPartsImageProvider(m_document));

}

QHash<int, QByteArray> LOPartsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "index";
    roles[NameRole] = "name";

    return roles;
}

int LOPartsModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_entries.count();
}

QVariant LOPartsModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() > m_entries.count())
        return QVariant();

    const LOPartEntry &part = m_entries.at(index.row());

    switch (role) {
    case IndexRole:
        return part.index;
    case NameRole:
        return part.name;

    default:
        return 0;
    }
}

QVariantMap LOPartsModel::get(int index) const
{
    if (index < 0 || index > m_entries.count() - 1) {
        qWarning() << Q_FUNC_INFO << "Index not valid, return undefined";
        return QVariantMap();
    }

    const LOPartEntry &part = m_entries.at(index);

    QVariantMap map;
    map["name"] = part.name;
    map["index"] = part.index;

    return map;
}

void LOPartsModel::fillModel() {
    if (m_document) {
        if (!m_entries.isEmpty()) {
            beginRemoveRows(QModelIndex(), 0, rowCount());
            m_entries.clear();
            endRemoveRows();
        }

        int partsCount = m_document->partsCount();

        for (int i = 0; i < partsCount; i++) {
            LOPartEntry part;
            part.index = i;
            part.name = m_document->getPartName(i);

            beginRemoveRows(QModelIndex(), rowCount(), rowCount());
            m_entries.append(part);
            endRemoveRows();
        }

        Q_EMIT countChanged();
    }    
}

LOPartsModel::~LOPartsModel()
{
}
