/*
    MidenEngine is a full C++ 3D Engine written on top of SDL2
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

#ifndef MESH_H
#define MESH_H

#include "includes.h"
#include <fstream>
#include <sstream>
#include <QString>

#include "vertex.h"
#include "texture.h"

class Mesh
{
public:
    Texture *tex;
    QList<Vertex *>v;
    QList<int>t;
    Mesh(QList<Vertex *> v, QList<int> t);
    Mesh(QList<Vertex *> v, QList<int> t, Texture *tex);

    //transformation
    void translate(double x, double y, double z);
    void rotateX(double angle, Vec3 origin = Vec3(0,0,0));
    void rotateY(double angle, Vec3 origin = Vec3(0,0,0));
    void rotateZ(double angle, Vec3 origin = Vec3(0,0,0));
    void scale(double x, double y, double z, Vec3 origin = Vec3(0,0,0));

    Vec3 getCenter();
    static Mesh *importMesh(string objPath, Texture *tex);
};

#endif // MESH_H
