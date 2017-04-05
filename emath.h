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

#ifndef EMATH_H
#define EMATH_H

class EMath
{
public:
    static double radians(double angle);
    static double max(double a, double b);
    static double min(double a, double b);
    static double abs(double x);
    static double lerp(double a, double b, double mix);
    static double map(double value, double min, double max, double newMin, double newMax);
    static double normalize(double value, double zero, double one);
    static int ceil(double x);
};

#endif // EMATH_H
