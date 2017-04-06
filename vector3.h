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

#ifndef VEC3_H
#define VEC3_H

#include "includes.h"

#include "emath.h"

class Vec3
{
public:
    double x;
    double y;
    double z;

    Vec3(double x, double y, double z);
    void        set(Vec3 v);
    void        set(Vec3 *v);
    double      length();
    void        translate(double x, double y, double z);
    void        rotateX(double angle, Vec3 o);
    void        rotateY(double angle, Vec3 o);
    void        rotateZ(double angle, Vec3 o);
    void        scale(double x, double y, double z, Vec3 o);
    std::string toString();
    Vec3 *copy();

    static Vec3 addVectors(Vec3 v0, Vec3 v1);
    static Vec3 subVectors(Vec3 v0, Vec3 v1);
    static Vec3 mulVectors(Vec3 v0, Vec3 v1);
    static Vec3 divVectors(Vec3 v0, Vec3 v1);

    static Vec3 addScalar(Vec3 v, double s);
    static Vec3 subScalar(Vec3 v, double s);
    static Vec3 mulScalar(Vec3 v, double s);
    static Vec3 divScalar(Vec3 v, double s);

    static double  dot(Vec3 v0, Vec3 v1);
    static Vec3 cross(Vec3 v0, Vec3 v1);
    static Vec3 normalize(Vec3 v);
    static Vec3 velocity(Vec3 v0,Vec3 v1,double delta);

    static double  cross2D(Vec3 v0, Vec3 v1);
    static double  triangleAreaTimesTwo(Vec3 v0,Vec3 v1,Vec3 v2);
    static Vec3 *toScreenSpace(Vec3 *v, double width);
    static Vec3 ceilVector(Vec3 v);
    static Vec3 *lerp(Vec3 *a, Vec3 *b, double mix);

};

#endif // VEC3_H
