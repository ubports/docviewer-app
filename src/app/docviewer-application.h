/*
 * Copyright (C) 2012 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 * Charles Lindsay <chaz@yorba.org>
 */

#ifndef DOCVIEWERAPPLICATION_H
#define DOCVIEWERAPPLICATION_H

#include <QApplication>

class QQuickView;
class DocViewerApplication : public QApplication
{
    Q_OBJECT

public:
    explicit DocViewerApplication(int& argc, char** argv);
    virtual ~DocViewerApplication();

    bool init();
    int exec();

private:
    void registerQML();
    void createView();

    QQuickView *m_view;
};

#endif // DOCVIEWERAPPLICATION_H
