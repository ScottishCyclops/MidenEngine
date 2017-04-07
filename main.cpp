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
#include "texture.h"

const float second = 1000.f;
const string resourcesFolder = ".././resources";
const double factor = .1;
const double rotFac = 10;

struct Game
{
    Display *display;
    SDL_Event event;
    bool running;
    uint fpsCap;
};

Vec3 getMousePosition();

int main()
{
    //creating the display
    Display display(800,800,"Miden Engine");

    //openning the display
    display.open();
    if(!display.isOpen)
        return 1;

    double size = .5;
    //test vertex
    Vertex v0(0,size,0,  255,0,0,0,  .5,0);
    Vertex v1(-size,-size,0, 0,255,0,0,  0,1);
    Vertex v2(size,-size,0,  0,0,255,0,  1,1);

    //test texture
    Texture tex(resourcesFolder+"/suzanne.bmp",display.buffer);


    //test mesh
    Mesh *m = Mesh::importMesh(resourcesFolder+"/suzanne.obj",&tex);
    m->scale(1.5,1.5,1.5,m->getCenter());


    Game game = {&display, 0, true, 1000};
    Renderer render(&display,150);
    int index = 0;
    uint frames = 0;

    //gettings time in ms since app start
    uint startTime = SDL_GetTicks();
    uint lastTime = startTime;

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
                //KEYBOARD events
                switch(game.event.key.keysym.sym)
                {
                case SDLK_UP:    m->translate(0,factor,0);  break;
                case SDLK_DOWN:  m->translate(0,-factor,0); break;
                case SDLK_LEFT:  m->translate(-factor,0,0); break;
                case SDLK_RIGHT: m->translate(factor,0,0);  break;
                case SDLK_w:     m->translate(0,0,factor); break;
                case SDLK_a:     m->rotateY(rotFac,m->getCenter());        break;
                case SDLK_s:     m->translate(0,0,-factor);  break;
                case SDLK_d:     m->rotateY(-rotFac,m->getCenter());       break;

                case SDLK_ESCAPE: game.running = false; break;
                case SDLK_TAB: index == 0 ? index = 1 : index  = 0; break;
                default : printf("OTHER\n"); break;
                }
            }
            else if(game.event.type == SDL_MOUSEMOTION)
            {
                //MOUSE events
            }
        }

        //Rendering
        render.clearTarget();

        //m->rotateY(.5,m->getCenter());


        render.drawMesh(m);
        //render.drawTex(&tex);

        display.swap();
        frames++;

        //FPS output
        if(SDL_GetTicks()-lastTime >= second)
        {
            printf("FPS : %d\n",frames);
            lastTime = SDL_GetTicks();
            frames = 0;
        }

        //framerate cap
        //SDL_Delay(second/game.fpsCap);
    }


    //quitting SDL and freeing the display
    tex.free();
    display.close();
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

