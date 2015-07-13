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
    TileItem(const QRect &area, LODocument* document);
    ~TileItem();

    QRect area() const;
    void setArea(const QRect &area);

    QImage texture() const;

    bool isPainted() const;
    void setPainted(bool isPainted);

    LODocument* document() const;
    void setDocument(LODocument* document);

public Q_SLOTS:
    void requestTexture();
    void releaseTexture();

Q_SIGNALS:
    void areaChanged();
    void textureChanged();
    void isPaintedChanged();
    void documentChanged();

private Q_SLOTS:
    void updateTexture(QImage t);

private:
    QRect m_area;
    QImage m_texture;
    bool m_painted;

    LODocument* m_document;
};

class RenderTask : public QObject, public QRunnable
{
    Q_OBJECT

public:
    RenderTask(const QRect &area, LODocument* document);

    QRect area() const;
    void setArea(const QRect &area);

    LODocument* document() const;
    void setDocument(LODocument* document);

    void run();

Q_SIGNALS:
    void areaChanged();
    void documentChanged();
    void renderCompleted(QImage t);

private:
    QRect m_area;
    LODocument* m_document;
};

#endif // TILEITEM_H
