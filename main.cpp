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

#include "includes.h"

#include "display.h"
#include "vertex.h"
#include "renderer.h"
#include "mesh.h"

const string resourcesFolder = ".././resources";

struct Game
{
    Display *display;
    SDL_Event event;
    bool running;
    uint fpsCap;
};

Vec3 getMousePosition();
SDL_Surface *loadImage(string path, SDL_Surface *reference);

int main()
{
    //creating the display
    Display display(800,800,"Miden Engine");



    //openning the display
    display.open();
    if(!display.isOpen)
        return 1;

    Vertex v0(100,100,0, 255,0,0);
    Vertex v1(100,300,0, 0,255,0);
    Vertex v2(300,300,0, 0,0,255);


    Mesh m({&v0,&v1,&v2},{0,1,2});

    Uint32 black = SDL_MapRGB(display.buffer->format, 0,0,0);

    Game game = {&display, 0, true, 1000};
    Renderer render(&display);
    int index = 0;

    uint startTime = SDL_GetTicks();
    uint lastTime = startTime;

    uint frames = 0;
    //Game loop
    while(game.running)
    {

        //Event loop
        while(SDL_PollEvent(&game.event) != 0)
        {
            if(game.event.type == SDL_QUIT)
            {
                game.running = false;
            }
            else if(game.event.type == SDL_KEYDOWN)
            {
                switch(game.event.key.keysym.sym)
                {
                case SDLK_UP: printf("UP\n"); break;
                case SDLK_DOWN: printf("DOWN\n"); break;
                case SDLK_LEFT: printf("LEFT\n"); break;
                case SDLK_RIGHT: printf("RIGHT\n"); break;
                case SDLK_ESCAPE: game.running = false; break;
                case SDLK_TAB: index == 0 ? index = 1 : index  = 0; break;
                default : printf("OTHER\n"); break;
                }
            }
            else if(game.event.type == SDL_MOUSEMOTION)
            {
                if(index == 1)
                    m.vbo[index]->location->set(getMousePosition());
            }
        }

        //Rendering
        SDL_FillRect(display.buffer,NULL,black);

        //render.drawPoint(m.vbo[0]);
        //render.drawLine(m.vbo[0],m.vbo[1]);
        render.drawTriangle(m.vbo[m.ibo[0]],m.vbo[m.ibo[1]],m.vbo[m.ibo[2]]);

        /*
        if(index == 0)
            m.vbo[1]->location->rotateZ(.3,*m.vbo[0]->location);
            */

        display.swap();
        frames++;

        //FPS output
        if(SDL_GetTicks()-lastTime >= 1000)
        {
            printf("FPS : %d\n",frames);
            lastTime = SDL_GetTicks();
            frames = 0;
        }

        SDL_Delay(1000.f/game.fpsCap);
    }


    display.close();
}

/**
 * @brief Loads an image as an SDL surface and optimizes it
 * @param path : the path to the BMP file
 * @param display : the display to opzimize for
 * @return a pointer to the SDL Surface, or NULL pointer
 */
SDL_Surface *loadImage(string path, SDL_Surface *reference)
{
    SDL_Surface *img = NULL;

    //importing raw image
    SDL_Surface *raw = SDL_LoadBMP(path.c_str());
    if(raw == NULL)
    {
        printf("Error loading image : %s\n",SDL_GetError());
    }
    else
    {
        //optimizing image to fit display surface format
        img = SDL_ConvertSurface(raw, reference->format, 0);
        if(img == NULL)
        {
            printf("Error converting image : %s\n",SDL_GetError());
        }

        //destroying the raw image
        SDL_FreeSurface(raw);
    }
    return img;
}

/**
 * @brief gets the mouse position though sdl
 * @return the mouse position as a vector
 */
Vec3 getMousePosition()
{
    int x,y = 0;

    SDL_GetMouseState(&x,&y);

    return Vec3(x,y,0);
}

