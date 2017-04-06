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

#ifndef TEXTURE_H
#define TEXTURE_H

#include "includes.h"

#include "vector3.h"
#include "color.h"
#include "emath.h"

class Texture
{
private:
    SDL_Surface *m_surface;
    SDL_Surface *m_ref;
    void loadBMP(string path);
    void optimizeTexture(SDL_Surface *reference);
    Uint8 *rawPixel(uint x,uint y);
public:
    string path;
    uint width;
    uint height;
    Texture(string path, SDL_Surface *reference);
    void reload();
    Uint32 *getPixelAt(Vec3 *coord);
};

#endif // TEXTURE_H
