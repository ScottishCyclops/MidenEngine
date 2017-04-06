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

#include "display.h"

Display::Display(unsigned int width, unsigned int height, string title)
{
    Display::width = width;
    Display::height = height;
    Display::title = title;

    Display::window = NULL;
    Display::surface = NULL;
    Display::buffer = SDL_CreateRGBSurface(0,Display::width,Display::height,32,0,0,0,0);
    Display::isOpen = false;
}

/**
 * @brief Fills the given display with pointers
 * @param display : the display structure to open
 * @return if it succeded or not
 */
bool Display::open()
{
    bool success = false;

    if(!Display::isOpen)
    {
        success = true;

        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("%s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //creating the sdl window
            Display::window = SDL_CreateWindow(Display::title.c_str(),
                                               SDL_WINDOWPOS_UNDEFINED,
                                               SDL_WINDOWPOS_UNDEFINED,
                                               Display::width,
                                               Display::height,
                                               SDL_WINDOW_SHOWN);
            if(Display::window == NULL)
            {
                printf("%s\n", SDL_GetError());
                success = false;
            }
            else
            {
                Display::surface = SDL_GetWindowSurface(Display::window);
            }
        }
        Display::isOpen = success;
    }

    return success;
}

/**
 * @brief Frees the resources of a display
 * @param display : the display to close
 */
void Display::close()
{
    if(Display::isOpen)
    {
        //freeing the surface
        SDL_FreeSurface(Display::surface);
        Display::surface = NULL;

        //destroying the window
        SDL_DestroyWindow(Display::window);
        Display::window = NULL;

        SDL_FreeSurface(Display::buffer);
        Display::buffer = NULL;

        Display::isOpen = false;

        //quitting SDL
        SDL_Quit();
    }
}

void Display::swap()
{
    SDL_BlitScaled(Display::buffer, NULL, Display::surface, NULL);
    SDL_UpdateWindowSurface(Display::window);
}
