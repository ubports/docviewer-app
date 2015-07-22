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
 */

#ifndef CONFIG_H
#define CONFIG_H

// This is the hardcoded Ubuntu/Debian paths to find the LibreOffice
// installation. If you want to use a parallel installation, change the path
// in the following line.
#define LO_PATH "/usr/lib/libreoffice/program/"

// FIXME: Perhaps we want to use smaller tiles on mobile devices?
#define TILE_SIZE 256.0

// Uncomment it if you want to see tiles boundaries
//#define DEBUG_SHOW_TILE_BORDER

// Uncomment for benchmarking tile rendering performance
//#define DEBUG_TILE_BENCHMARK

// Uncomment if you want more verbose application output
//#define DEBUG_VERBOSE

#endif // CONFIG_H
