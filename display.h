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

#ifndef DISPLAY_H
#define DISPLAY_H

#include "includes.h"

class Display
{
public:
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Surface *buffer;
    unsigned int width;
    unsigned int height;
    string title;
    bool isOpen;

    Display(unsigned int width, unsigned int height, string title);
    bool open();
    void close();
    void swap();
};

#endif // DISPLAY_H
