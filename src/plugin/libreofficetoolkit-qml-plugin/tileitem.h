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

#ifndef TILEITEM_H
#define TILEITEM_H

#include <QRunnable>
#include <QImage>

class LODocument;

class TileItem : public QObject
{
    Q_OBJECT

public:
    TileItem(QRect a, LODocument* doc);
    ~TileItem();

    void releaseTexture();

    QRect area;
    QImage texture;
    bool painted;

    LODocument* document;

Q_SIGNALS:
    void textureUpdated();

private Q_SLOTS:
    void updateTexture(QImage t);
};

class RenderTask : public QObject, public QRunnable
{
    Q_OBJECT

public:
    RenderTask(QRect area, LODocument* doc);
    void run();

Q_SIGNALS:
    void renderCompleted(QImage t);

private:
    QRect m_area;
    LODocument* m_document;
};

#endif // TILEITEM_H
