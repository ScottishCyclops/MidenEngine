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

#include "vector3.h"

//! creates a 3D vector. can also be used for 2D vector
Vec3::Vec3(double x = 0., double y = 0., double z = 0.)
{
    Vec3::x = x;
    Vec3::y = y;
    Vec3::z = z;
}

//! sets the vector values to thoses of another vector
void Vec3::set(Vec3 v)
{
    Vec3::x = v.x;
    Vec3::y = v.y;
    Vec3::z = v.z;
}

//! sets the vector values to thoses of a pointer vector
void Vec3::set(Vec3 *v)
{
    Vec3::x = v->x;
    Vec3::y = v->y;
    Vec3::z = v->z;
}

//! returns the length of the vector
double Vec3::length()
{
    return sqrt(Vec3::x*Vec3::x + Vec3::y*Vec3::y + Vec3::z*Vec3::z);
}

//! translates a vector by the given amount
void Vec3::translate(double x, double y, double z)
{
    Vec3::x+=x;
    Vec3::y+=y;
    Vec3::z+=z;
}

//! rotates the vector on x from the given origin
void Vec3::rotateX(double angle, Vec3 o)
{
    double rad = EMath::radians(angle);
    double angleSin = sin(rad);
    double angleCos = cos(rad);

    Vec3::set(Vec3::subVectors(*Vec3::copy(),o));
    double tmpY = (angleCos * Vec3::y) - (angleSin * Vec3::z);
    double tmpZ = (angleSin * Vec3::y) + (angleCos * Vec3::z);

    Vec3::y = tmpY;
    Vec3::z = tmpZ;
    Vec3::set(Vec3::addVectors(*Vec3::copy(),o));
}

//! rotates the vector on y from the given origin
void Vec3::rotateY(double angle, Vec3 o)
{
    double rad = EMath::radians(angle);
    double angleSin = sin(rad);
    double angleCos = cos(rad);

    Vec3::set(Vec3::subVectors(*Vec3::copy(),o));
    double tmpX =  (angleCos * Vec3::x) + (angleSin * Vec3::z);
    double tmpZ = -(angleSin * Vec3::x) + (angleCos * Vec3::z);

    Vec3::x = tmpX;
    Vec3::z = tmpZ;
    Vec3::set(Vec3::addVectors(*Vec3::copy(),o));
}

//! rotates the vector on z from the given origin
void Vec3::rotateZ(double angle, Vec3 o)
{
    double rad = EMath::radians(angle);
    double angleSin = sin(rad);
    double angleCos = cos(rad);

    Vec3::set(Vec3::subVectors(*Vec3::copy(),o));
    double tmpX = (angleCos * Vec3::x) - (angleSin * Vec3::y);
    double tmpY = (angleSin * Vec3::x) + (angleCos * Vec3::y);

    Vec3::x = tmpX;
    Vec3::y = tmpY;
    Vec3::set(Vec3::addVectors(*Vec3::copy(),o));
}

//! scales the vector from the given origin
void Vec3::scale(double x, double y, double z, Vec3 o)
{
    Vec3::set(Vec3::subVectors(*Vec3::copy(),o));
    Vec3::x*=x;
    Vec3::y*=y;
    Vec3::z*=z;
    Vec3::set(Vec3::addVectors(*Vec3::copy(),o));
}

//! returns a string representation of the vector
std::string Vec3::toString()
{
    return "(x:" + std::to_string(Vec3::x) + ",y:" + std::to_string(Vec3::y) + ",z:" + std::to_string(Vec3::z) + ")";
}

//! returns a copy of the vector
Vec3 *Vec3::copy()
{
    return new Vec3(x,y,z);
}

Vec3 Vec3::addVectors(Vec3 v0, Vec3 v1)
{
    return Vec3(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z);
}

Vec3 Vec3::subVectors(Vec3 v0, Vec3 v1)
{
    return Vec3(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z);
}

Vec3 Vec3::mulVectors(Vec3 v0, Vec3 v1)
{
    return Vec3(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z);
}

Vec3 Vec3::divVectors(Vec3 v0, Vec3 v1)
{
    return Vec3(v0.x/v1.x, v0.y/v1.y, v0.z/v1.z);
}


Vec3 Vec3::addScalar(Vec3 v, double s)
{
    return Vec3(v.x+s, v.y+s, v.z+s);
}

Vec3 Vec3::subScalar(Vec3 v, double s)
{
    return Vec3(v.x-s, v.y-s, v.z-s);
}

Vec3 Vec3::mulScalar(Vec3 v, double s)
{
    return Vec3(v.x*s, v.y*s, v.z*s);
}

Vec3 Vec3::divScalar(Vec3 v, double s)
{
    return Vec3(v.x/s, v.y/s, v.z/s);
}

double Vec3::dot(Vec3 v0, Vec3 v1)
{
    return v0.x*v1.x+v0.y*v1.y+v0.z*v1.z;
}

Vec3 Vec3::cross(Vec3 v0, Vec3 v1)
{
    return Vec3(v0.y*v1.z-v0.z*v1.y, v0.z*v1.x-v0.x*v1.z, v0.x*v1.y-v0.y*v1.x);
}

Vec3 Vec3::normalize(Vec3 v)
{
    return Vec3::divScalar(v,v.length());
}

Vec3 Vec3::velocity(Vec3 v0,Vec3 v1,double delta)
{
    return Vec3::divScalar(Vec3::subVectors(v1,v0),delta);
}

double Vec3::cross2D(Vec3 v0, Vec3 v1)
{
    return v0.x*v1.y-v1.x*v0.y;
}

double Vec3::triangleAreaTimesTwo(Vec3 v0,Vec3 v1,Vec3 v2)
{
    Vec3 delta1 = Vec3::subVectors(v1,v0);
    Vec3 delta2 = Vec3::subVectors(v2,v0);
    return Vec3::cross2D(delta1,delta2);
}

Vec3 *Vec3::toScreenSpace(Vec3 *v, double width)
{
    //to keep proportions, we scale based on the width of the screen,
    //not width and height
    //return new Vec3(EMath::map(v->x,0,1,0,width),EMath::map(v->y,0,1,width,0));
    double widthOverTwo = width/2;
    return new Vec3(v->x*widthOverTwo+widthOverTwo,v->y*(-widthOverTwo)+widthOverTwo);
}

Vec3 Vec3::ceilVector(Vec3 v)
{
    return Vec3(ceil(v.x),ceil(v.y),ceil(v.z));
}

Vec3 *Vec3::lerp(Vec3 *a, Vec3 *b, double mix)
{
    return new Vec3(EMath::lerp(a->x,b->x,mix),
                    EMath::lerp(a->y,b->y,mix),
                    EMath::lerp(a->z,b->z,mix));
}
