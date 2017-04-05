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

#include "color.h"

Color::Color(uint r, uint g, uint b, uint a)
{
    Color::r = r;
    Color::g = g;
    Color::b = b;
    Color::a = a;
}

Color Color::lerp(Color *x, Color *y, double mix)
{
    Color newColor(0,0,0,0);

    newColor.r = EMath::lerp(x->r, y->r, mix);
    newColor.g = EMath::lerp(x->g, y->g, mix);
    newColor.b = EMath::lerp(x->b, y->b, mix);
    newColor.a = EMath::lerp(x->a, y->a, mix);

    return newColor;
}
