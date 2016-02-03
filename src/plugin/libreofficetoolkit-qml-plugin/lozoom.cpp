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

#include "lozoom.h"
#include "loview.h"
#include "twips.h"

static qreal getZoomToFitWidth(const qreal &width, int documentWidth)
{
    return qreal(width / Twips::convertTwipsToPixels(documentWidth, 1.0));
}

static qreal getZoomToFitHeight(const qreal &height, int documentHeight)
{
    return qreal(height / Twips::convertTwipsToPixels(documentHeight, 1.0));
}

LOZoom::LOZoom(LOView *view)
    : QObject(view)
    , m_view(view)
    , m_zoomMode(ZoomMode::Manual)
    , m_zoomModesAvailable(ZoomMode::Manual)
    , m_zoomFactor(1.0)
    , m_minimumZoom(0.25)
    , m_maximumZoom(4.0)
{   }

LOZoom::~LOZoom()
{   }

LOZoom::ZoomMode LOZoom::zoomMode() const
{
    return m_zoomMode;
}

void LOZoom::setZoomMode(const LOZoom::ZoomMode zoomMode)
{
    if (m_zoomMode == zoomMode)
        return;

    m_zoomMode = zoomMode;
    Q_EMIT zoomModeChanged();
}

LOZoom::ZoomModes LOZoom::zoomModesAvailable() const
{
    return m_zoomModesAvailable;
}

void LOZoom::setZoomModesAvailability()
{
    if (!m_view->document())
        return;

    ZoomModes newZoomModesAvailable;
    newZoomModesAvailable |= ZoomMode::Manual;

    switch (m_view->document()->documentType()) {
    case LODocument::DocumentType::TextDocument:
        newZoomModesAvailable |= ZoomMode::FitToWidth;
        break;
    case LODocument::DocumentType::SpreadsheetDocument:
        break;
    default:
        newZoomModesAvailable |= ZoomMode::FitToWidth;
        newZoomModesAvailable |= ZoomMode::FitToHeight;
        newZoomModesAvailable |= ZoomMode::Automatic;
        break;
    }

    if (m_zoomModesAvailable != newZoomModesAvailable) {
        m_zoomModesAvailable = newZoomModesAvailable;
        Q_EMIT zoomModesAvailableChanged();
    }
}

qreal LOZoom::zoomFactor() const
{
    return m_zoomFactor;
}

void LOZoom::setZoomFactor(const qreal zoom)
{
    if (m_zoomFactor == zoom || zoom < m_minimumZoom || zoom > m_maximumZoom)
        return;

    m_zoomFactor = zoom;

    // m_valueAutomaticZoom is necessary equal to the fitToWidth or fitToHeight value.
    if (m_zoomFactor != m_valueFitToWidthZoom && m_zoomFactor != m_valueFitToHeightZoom)
        setZoomMode(LOZoom::Manual);

    Q_EMIT zoomFactorChanged();
}

qreal LOZoom::minimumZoom() const
{
    return m_minimumZoom;
}

/*
void LOZoom::setMinimumZoom(const qreal newValue)
{
    if (m_minimumZoom == newValue)
        return;

    m_minimumZoom = newValue;
    Q_EMIT minimumZoomChanged();
}
*/

qreal LOZoom::maximumZoom() const
{
    return m_maximumZoom;
}

/*
void LOZoom::setMaximumZoom(const qreal newValue)
{
    if (m_maximumZoom == newValue)
        return;

    m_maximumZoom = newValue;
    Q_EMIT maximumZoomChanged();
}
*/

qreal LOZoom::valueFitToWidthZoom() const
{
    return m_valueFitToWidthZoom;
}

qreal LOZoom::valueFitToHeightZoom() const
{
    return m_valueFitToHeightZoom;
}

qreal LOZoom::valueAutomaticZoom() const
{
    return m_valueAutomaticZoom;
}

bool LOZoom::adjustZoomToWidth(bool changeMode)
{
    if (!m_view->document())
        return false;

    if (changeMode)
        setZoomMode(LOZoom::FitToWidth);

    int currentPart = m_view->currentPart();

    m_valueFitToWidthZoom = getZoomToFitWidth(m_view->parentFlickable()->width(),
                                              m_view->document()->documentSize(currentPart).width());

    Q_EMIT valueFitToWidthZoomChanged();

    if (m_zoomFactor != m_valueFitToWidthZoom) {
        setZoomFactor(m_valueFitToWidthZoom);

        qDebug() << Q_FUNC_INFO << "- value:" << m_zoomFactor << "- changeMode:" << changeMode;
        return true;
    }

    return false;
}

bool LOZoom::adjustZoomToHeight(bool changeMode)
{
    if (!m_view->document())
        return false;

    if (changeMode)
        setZoomMode(LOZoom::FitToHeight);

    int currentPart = m_view->currentPart();

    m_valueFitToHeightZoom = getZoomToFitHeight(m_view->parentFlickable()->height(),
                                                m_view->document()->documentSize(currentPart).height());

    Q_EMIT valueFitToHeightZoomChanged();

    if (m_zoomFactor != m_valueFitToHeightZoom) {
        setZoomFactor(m_valueFitToHeightZoom);

        qDebug() << Q_FUNC_INFO << "- value:" << m_zoomFactor << "- changeMode:" << changeMode;
        return true;
    }

    return false;
}

bool LOZoom::adjustAutomaticZoom(bool changeMode)
{
    if (!m_view->document())
        return false;

    if (changeMode)
        setZoomMode(LOZoom::Automatic);

    int currentPart = m_view->currentPart();

    m_valueFitToWidthZoom = getZoomToFitWidth(m_view->parentFlickable()->width(),
                                              m_view->document()->documentSize(currentPart).width());

    m_valueFitToHeightZoom = getZoomToFitHeight(m_view->parentFlickable()->height(),
                                                m_view->document()->documentSize(currentPart).height());

    m_valueAutomaticZoom = qMin(m_valueFitToWidthZoom, m_valueFitToHeightZoom);

    Q_EMIT valueFitToWidthZoomChanged();
    Q_EMIT valueFitToHeightZoomChanged();
    Q_EMIT valueAutomaticZoomChanged();

    if (m_zoomFactor != m_valueAutomaticZoom) {
        setZoomFactor(m_valueAutomaticZoom);

        qDebug() << Q_FUNC_INFO << "- value:" << m_zoomFactor << "- changeMode:" << changeMode;
        return true;
    }

    return false;
}

void LOZoom::init()
{
    setZoomModesAvailability();

    switch (m_view->document()->documentType()) {
    case LODocument::DocumentType::SpreadsheetDocument:
        setZoomMode(ZoomMode::Manual);
        setZoomFactor(1.0);
        break;
    case LODocument::DocumentType::PresentationDocument:
        setZoomMode(ZoomMode::Automatic);
        break;
    default:
        setZoomMode(ZoomMode::FitToWidth);
        break;
    }
}
