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
#include "config.h"

#include <QThreadPool>
#include <QDebug>

#ifdef DEBUG_TILE_BENCHMARK
#include <QElapsedTimer>
#endif

/* ****************
 * TileItem class *
 ******************/

TileItem::TileItem(const QRect &area, LODocument *document)
    : m_painted(false)
    , m_document(nullptr)
{
    this->setArea(area);
    this->setDocument(document);
}

// Destructor
TileItem::~TileItem()
{
    this->releaseTexture();
}

QRect TileItem::area() const
{
    return m_area;
}

void TileItem::setArea(const QRect &area)
{
    if (m_area == area)
        return;

    m_area = area;
    Q_EMIT areaChanged();
}

QImage TileItem::texture() const
{
    return m_texture;
}

bool TileItem::isPainted() const
{
    return m_painted;
}

void TileItem::setPainted(bool isPainted)
{
    if (m_painted == isPainted)
        return;

    m_painted = isPainted;
    Q_EMIT isPaintedChanged();
}

LODocument* TileItem::document() const
{
    return m_document;
}

void TileItem::setDocument(LODocument* document)
{
    if (m_document == document)
        return;

    m_document = document;
    Q_EMIT documentChanged();
}

void TileItem::requestTexture()
{
    auto task = new RenderTask(this->area(), this->document());
    connect(task, SIGNAL(renderCompleted(QImage)), this, SLOT(updateTexture(QImage)));

    task->setAutoDelete(true);
    QThreadPool::globalInstance()->start(task);
}

// Free memory used by the texture
void TileItem::releaseTexture()
{
    if (m_texture.isNull())
        return;

    m_texture = QImage();
    Q_EMIT textureChanged();
}

// This is a slot, connect to renderCompleted() signal from RenderTask class.
void TileItem::updateTexture(QImage t)
{
    m_texture = t;
    Q_EMIT textureChanged();
}

/* ******************
 * RenderTask class *
 ********************/

RenderTask::RenderTask(const QRect &area, LODocument* document)
{
    this->setArea(area);
    this->setDocument(document);
}


QRect RenderTask::area() const
{
    return m_area;
}

void RenderTask::setArea(const QRect &area)
{
    if (m_area == area)
        return;

    m_area = area;
    Q_EMIT areaChanged();
}

LODocument* RenderTask::document() const
{
    return m_document;
}

void RenderTask::setDocument(LODocument* document)
{
    if (m_document == document)
        return;

    m_document = document;
    Q_EMIT documentChanged();
}

// Render the texture for this tile.
void RenderTask::run()
{
    // Can cause crash if document is nullptr.
    if (!m_document)
        return;

#ifdef DEBUG_TILE_BENCHMARK
    QElapsedTimer renderTimer;
    renderTimer.start();
#endif

    QImage render = m_document->paintTile(this->area().size(),
                                                this->area());

    Q_EMIT renderCompleted(render);

#ifdef DEBUG_TILE_BENCHMARK
    qDebug() << "Time to render the tile:" << renderTimer.elapsed() << "ms";
#endif
}
