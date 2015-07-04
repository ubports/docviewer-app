/*
 * Copyright (C) 2015 Canonical, Ltd.
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
 * Author: Stefano Verzegnassi <stefano92.100@gmail.com>
 */

#include "tileitem.h"
#include "lodocument.h"

#include <QThreadPool>
#include <QDebug>

/* ****************
 * TileItem class *
 ******************/

TileItem::TileItem(QRect a, LODocument *doc)
    : painted(false)
{
    area = a;

    RenderTask* task = new RenderTask(area, doc);
    connect(task, SIGNAL(renderCompleted(QImage)), this, SLOT(updateTexture(QImage)));

    task->setAutoDelete(true);
    QThreadPool::globalInstance()->start(task);
}

// Destructor
TileItem::~TileItem()
{
    this->releaseTexture();
}

// Free memory used by the texture
void TileItem::releaseTexture()
{
    if (texture.isNull())
        return;

    texture = QImage();
}

// This is a slot, connect to renderCompleted() signal from RenderTask class.
void TileItem::updateTexture(QImage t)
{
    qDebug() << "Updating texture";
    texture = t;
    Q_EMIT textureUpdated();
}

/* ******************
 * RenderTask class *
 ********************/

RenderTask::RenderTask(QRect area, LODocument *doc)
{
    m_document = doc;
    m_area = area;
}

// Render the texture for this tile.
void RenderTask::run()
{
    QImage render = m_document->paintTile(m_area.size(), m_area);
    Q_EMIT renderCompleted(render);
}
