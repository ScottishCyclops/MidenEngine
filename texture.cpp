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

#include "texture.h"

Texture::Texture(string path, SDL_Surface *reference)
{
    Texture::path = path;
    Texture::m_ref = reference;
    Texture::m_surface = NULL;

    Texture::loadBMP(Texture::path);
    Texture::optimizeTexture(Texture::m_ref);
}

void Texture::loadBMP(string path)
{
    if(Texture::m_surface != NULL)
    {
        SDL_FreeSurface(Texture::m_surface);
        Texture::m_surface = NULL;
        Texture::width = 0;
        Texture::height = 0;
    }

    //importing raw image
    Texture::m_surface = SDL_LoadBMP(path.c_str());
    if(Texture::m_surface == NULL)
    {
        printf("Error loading image : %s\n",SDL_GetError());
    }
    else
    {
        Texture::width = Texture::m_surface->w;
        Texture::height = Texture::m_surface->h;
    }
}

void Texture::optimizeTexture(SDL_Surface *reference)
{
    if(Texture::m_surface != NULL)
    {
        //optimizing image to fit display surface format
        SDL_Surface *optimized = SDL_ConvertSurface(Texture::m_surface, reference->format, 0);
        if(optimized == NULL)
        {
            printf("Error converting image : %s\nReverted to original\n",SDL_GetError());
        }
        else
        {
            //destroying the old image
            SDL_FreeSurface(Texture::m_surface);
            Texture::m_surface = optimized;
        }
    }
    else
    {
        printf("No Texture loaded\n");
    }
}

void Texture::reload()
{
    Texture::loadBMP(Texture::path);
    Texture::optimizeTexture(Texture::m_ref);
}
/*
double getBilinearFilteredPixelColor(Texture tex, double u, double v) {
  u = u * tex.size - 0.5;
  v = v * tex.size - 0.5;
  int x = floor(u);
  int y = floor(v);
  double u_ratio = u - x;
  double v_ratio = v - y;
  double u_opposite = 1 - u_ratio;
  double v_opposite = 1 - v_ratio;
  double result = (tex[x][y]   * u_opposite  + tex[x+1][y]   * u_ratio) * v_opposite +
                  (tex[x][y+1] * u_opposite  + tex[x+1][y+1] * u_ratio) * v_ratio;
  return result;
}
*/

Uint32 *Texture::getPixelAt(Vec3 *coord)
{
    //texture origin is at the bottom left
    double u = coord->x * (Texture::width-1);
    double v = (coord->y *-1 +1) * (Texture::height-1);
    /*
    int x = (int)u;
    int y = (int)v;

    double uRatio = u-x;
    double vRatio = v-y;

    double uOpposite = 1-uRatio;
    double vOpposite = 1-vRatio;
    */

    return (Uint32 *) Texture::rawPixel((int)u%Texture::width,(int)v%Texture::height);
}

Uint8 *Texture::rawPixel(uint x, uint y)
{
    return (Uint8 *)Texture::m_surface->pixels + y * Texture::m_surface->pitch + x * bytesPerPixel;
}

void Texture::free()
{
    SDL_FreeSurface(Texture::m_surface);
}
