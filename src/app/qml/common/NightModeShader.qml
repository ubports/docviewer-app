/*
 * Copyright (C) 2015, 2016 Stefano Verzegnassi
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
            p.r = min(0.8, (1.0 - p.r) * 0.8 + 0.1);
            p.g = min(0.8, (1.0 - p.g) * 0.8 + 0.1);
            p.b = min(0.8, (1.0 - p.b) * 0.8 + 0.1);
            gl_FragColor = vec4(vec3(dot(p.rgb, vec3(0.299, 0.587, 0.114))), p.a) * qt_Opacity;
        }
    "
}

