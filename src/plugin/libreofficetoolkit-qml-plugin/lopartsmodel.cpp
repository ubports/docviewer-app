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

#include <QDebug>

LOPartsModel::LOPartsModel(const QSharedPointer<LODocument>& document, QAbstractListModel *parent):
    QAbstractListModel(parent)
{   
    m_document = document;
    fillModel();
}

QHash<int, QByteArray> LOPartsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "index";
    roles[NameRole] = "name";
    roles[IdRole] = "id";
    roles[ThumbnailRole] = "thumbnail";

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
    case IdRole:
        return part.id;
    case ThumbnailRole:
        return part.thumbnail;

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
    map["id"] = part.id;
    map["thumbnail"] = part.thumbnail;

    return map;
}

void LOPartsModel::notifyAboutChanges(int id)
{
    for (int i = 0; i < m_entries.size(); i++)
        if (m_entries[i].id == id) {
            m_entries[i].thumbnail += "/cached";
            Q_EMIT dataChanged(createIndex(i, 0), createIndex(i + 1, 0));
            break;
        }
}

void LOPartsModel::fillModel() {
    if (!m_document)
        return;

    if (!m_entries.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount());
        m_entries.clear();
        endRemoveRows();
    }

    int partsCount = m_document->partsCount();
    beginInsertColumns(QModelIndex(), 0, qMax(partsCount - 1, 0));
    for (int i = 0; i < partsCount; i++) {
        LOPartEntry part;

        part.index = i;
        part.name = m_document->getPartName(i);
        part.id = RenderEngine::getNextId();
        part.thumbnail = QString("image://lok/part/%1/%2").arg(QString::number(part.index)).arg(QString::number(part.id));

        m_entries.append(part);
    }
    endInsertColumns();

    Q_EMIT countChanged();
}

LOPartsModel::~LOPartsModel()
{ }
