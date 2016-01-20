/*
 * Copyright (C) 2016 Stefano Verzegnassi
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

#ifndef LOZOOM_H
#define LOZOOM_H

#include <QObject>
#include <QSharedPointer>

class LOView;

class LOZoom : public QObject
{
    Q_OBJECT
    Q_ENUMS(ZoomMode)
    Q_FLAGS(ZoomModes)
    Q_PROPERTY(ZoomMode zoomMode READ zoomMode NOTIFY zoomModeChanged)
    Q_PROPERTY(ZoomModes zoomModesAvailable READ zoomModesAvailable NOTIFY zoomModesAvailableChanged)
    Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged)
    Q_PROPERTY(qreal minimumZoom READ minimumZoom CONSTANT)     //WRITE setMinimumZoom NOTIFY minimumZoomChanged)
    Q_PROPERTY(qreal maximumZoom READ maximumZoom CONSTANT)     //WRITE setMaximumZoom NOTIFY maximumZoomChanged)
    Q_PROPERTY(qreal valueFitToWidthZoom READ valueFitToWidthZoom NOTIFY valueFitToWidthZoomChanged)
    Q_PROPERTY(qreal valueFitToHeightZoom READ valueFitToHeightZoom NOTIFY valueFitToHeightZoomChanged)
    Q_PROPERTY(qreal valueAutomaticZoom READ valueAutomaticZoom NOTIFY valueAutomaticZoomChanged)

public:
    LOZoom(LOView *view);
    ~LOZoom();

    enum ZoomMode {
        Manual = 0x0,
        FitToWidth = 0x1,
        FitToHeight = 0x2,
        Automatic = 0x4
    };
    Q_DECLARE_FLAGS(ZoomModes, ZoomMode)

    ZoomMode    zoomMode() const;
    ZoomModes   zoomModesAvailable() const;

    qreal       zoomFactor() const;
    void        setZoomFactor(const qreal zoom);

    qreal       minimumZoom() const;
//    void        setMinimumZoom(const qreal newValue);

    qreal       maximumZoom() const;
//    void        setMaximumZoom(const qreal newValue);

    qreal       valueFitToWidthZoom() const;
    qreal       valueFitToHeightZoom() const;
    qreal       valueAutomaticZoom() const;

    bool adjustZoomToWidth(bool changeMode = true);
    bool adjustZoomToHeight(bool changeMode = true);
    bool adjustAutomaticZoom(bool changeMode = true);

    void init();

Q_SIGNALS:
    void zoomModeChanged();
    void zoomModesAvailableChanged();
    void zoomFactorChanged();
//    void minimumZoomChanged();
//    void maximumZoomChanged();
    void valueFitToWidthZoomChanged();
    void valueFitToHeightZoomChanged();
    void valueAutomaticZoomChanged();

private:
    LOView* m_view;

    ZoomMode m_zoomMode;
    ZoomModes m_zoomModesAvailable;

    qreal m_zoomFactor;

    qreal m_minimumZoom;
    qreal m_maximumZoom;

    qreal m_valueFitToWidthZoom;
    qreal m_valueFitToHeightZoom;
    qreal m_valueAutomaticZoom;

private:
    void setZoomMode(const ZoomMode zoomMode);
    void setZoomModesAvailability();
};

#endif // LOZOOM_H
