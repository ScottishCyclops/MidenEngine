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

#include "emath.h"

//! converts an angle from degrees to radians
double EMath::radians(double angle)
{
    return angle*pi/180;

}

double EMath::max(double a, double b)
{
    return a >= b ? a : b;
}

double EMath::min(double a, double b)
{
    return a <= b ? a : b;
}

double EMath::abs(double x)
{
    return x >= 0 ? x : -x;
}

double EMath::lerp(double a, double b, double mix)
{
    return (1 - mix)*a + mix*b;
}

double EMath::normalize(double value, double zero, double one)
{
    return EMath::map(value,zero,one,0,1);
}

double EMath::map(double value, double min, double max, double newMin, double newMax)
{
    return (((value-min)/max)*newMax)+newMin;
}

int EMath::ceil(double x)
{
    //if removing the decimal part doesn't change
    // the number, we return it.
    // otherwise, we floor it and add 1
    return int(x) == x ? x : int(x)+1;
}
