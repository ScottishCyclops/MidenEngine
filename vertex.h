/*
    MidenEngine is a full C++ 3D Engine written on top of Xlib
    Copyright (C) 2017 Scott Winkelmann

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VERTEX_H
#define VERTEX_H

#include "vector3.h"
#include "color.h"


class Vertex
{
public:
    Vec3* location;
    Vec3* texCoord;
    Color* color;

    Vertex(double x, double y, double z, uint r, uint g, uint b, uint a = 255, double u = 0, double v = 0);
    double getX();
    double getY();
    double getZ();
    double getU();
    double getV();
};

#endif // VERTEX_H
