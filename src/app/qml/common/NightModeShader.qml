/*
 * Copyright (C) 2015 Canonical, Ltd.
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

import QtQuick 2.4

ShaderEffect {
    fragmentShader: "
        uniform lowp sampler2D source;
        uniform lowp float qt_Opacity;
        varying highp vec2 qt_TexCoord0;

        void main() {
            lowp vec4 p = texture2D(source, qt_TexCoord0);
            p.r = 1.0 - p.r;
            p.g = 1.0 - p.g;
            p.b = 1.0 - p.b;
            gl_FragColor = vec4(vec3(dot(p.rgb, vec3(0.344, 0.5, 0.156))), p.a) * qt_Opacity;
        }
    "
}

