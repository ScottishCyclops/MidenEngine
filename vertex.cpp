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

#include "vertex.h"

Vertex::Vertex(double x, double y, double z)
{
    Vertex::location = new Vec3(x,y,z);
    Vertex::texCoord = new Vec3(0,0,0);
}

Vertex::Vertex(double x, double y, double z, double u, double v)
{
    Vertex::location = new Vec3(x,y,z);
    Vertex::texCoord = new Vec3(u,v,0);
}

Vertex::Vertex(Vec3 *location)
{
    Vertex::location = location;
    Vertex::texCoord = new Vec3(0,0,0);
}

Vertex::Vertex(Vec3 *location, Vec3 *texCoord)
{
    Vertex::location = location;
    Vertex::texCoord = texCoord;
}

//getters

double Vertex::getX()
{
    return Vertex::location->x;
}

double Vertex::getY()
{
    return Vertex::location->y;
}

double Vertex::getZ()
{
    return Vertex::location->z;
}

double Vertex::getU()
{
    return Vertex::texCoord->x;
}

double Vertex::getV()
{
    return Vertex::texCoord->y;
}

Vertex *Vertex::copy()
{
    return new Vertex(Vertex::location->copy(),Vertex::texCoord->copy());
}
