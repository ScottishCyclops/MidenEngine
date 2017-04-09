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

#include "renderer.h"

// Edge //

Edge::Edge(Vertex *vert1, Vertex *vert2)
{
    //sort vertex by minimum y
    if(vert1->getY() < vert2->getY())
    {
        Edge::min = vert1;
        Edge::max = vert2;
    }
    else
    {
        Edge::min = vert2;
        Edge::max = vert1;
    }

    Edge::deltaX = Edge::max->getX()-Edge::min->getX();
    Edge::deltaY = Edge::max->getY()-Edge::min->getY();
}

// Span //

Span::Span(double x1, double x2, Vec3 *coord1, Vec3 *coord2)
{
    //sort xs by minimum
    if(x1 < x2)
    {
        Span::minX = x1;
        Span::maxX = x2;

        Span::minCoord = coord1;
        Span::maxCoord = coord2;
    }
    else
    {
        Span::minX = x2;
        Span::maxX = x1;

        Span::minCoord = coord2;
        Span::maxCoord = coord1;
    }

    Span::deltaX = Span::maxX-Span::minX;
    Span::deltaU = Span::maxCoord->x-Span::minCoord->x;
    Span::deltaV = Span::maxCoord->y-Span::minCoord->y;
}

// Renderer //

Renderer::Renderer(Display *display, double fov)
{
    Renderer::m_display = display;
    Renderer::m_backColor = 0;

    Renderer::m_fov = fov;
    Renderer::m_tanHalfFov = tan(EMath::radians(Renderer::m_fov/2));

    Renderer::m_rotation = new Vec3(0,0,0);
    Renderer::updateRotation();
}

void Renderer::updateRotation()
{
    //we ignore y because the camera never really rotates on its y
    double radX = EMath::radians(Renderer::m_rotation->x);
    double radZ = EMath::radians(Renderer::m_rotation->z);

    Renderer::m_rotSin = new Vec3(sin(radX),0,sin(radZ));
    Renderer::m_rotCos = new Vec3(cos(radX),1,cos(radZ));
}
/*
void Renderer::drawPoint(Vertex *vert)
{
    if(vert->getX() < Renderer::m_display->width && vert->getX() >= 0 && vert->getY() < Renderer::m_display->height && vert->getY() >= 0)
    {
        Renderer::putpixel(EMath::ceil(vert->getX()),
                           EMath::ceil(vert->getY()),
                           vert->color->toUint32(Renderer::m_display->buffer->format));
    }
}

void Renderer::drawLine(Vertex *vert1, Vertex *vert2)
{

    double deltaX = (vert2->getX() - vert1->getX());
    double deltaY = (vert2->getY() - vert1->getY());

    if(deltaX == 0 && deltaY == 0) {
        Renderer::drawPoint(vert1);
        return;
    }

    Vec3 *min, *max = NULL;
    Color *col1 = vert1->color;
    Color *col2 = vert2->color;
    if(EMath::abs(deltaX) > EMath::abs(deltaY))
    {
        // sort vertex by minimum x
        if(vert1->getX() < vert2->getX())
        {
            min = vert1->location;
            max = vert2->location;
        }
        else
        {
            min = vert2->location;
            max = vert1->location;
        }

        // draw line in terms of y slope
        double slope = deltaY / deltaX;

        for(double x = min->x; x <= max->x; x++)
        {

            double y = vert1->getY() + ((x - vert1->getX()) * slope);
            if(x < Renderer::m_display->width && x >= 0 && y < Renderer::m_display->height && y >= 0)
            {
                double colorMix = (x-vert1->getX())/deltaX;
                Color *color = Color::lerp(col1,col2,colorMix);
                Renderer::putpixel(EMath::ceil(x),EMath::ceil(y),color->toUint32(Renderer::m_display->buffer->format));
            }
        }
    }
    else
    {
        // sort vertex by minimum y
        if(vert1->getY() < vert2->getY())
        {
            min = vert1->location;
            max = vert2->location;
        }
        else
        {
            min = vert2->location;
            max = vert1->location;
        }

        // draw line in terms of x slope
        double slope = deltaX / deltaY;

        for(double y = min->y; y <= max->y; y++)
        {

            double x = vert1->getX() + ((y - vert1->getY()) * slope);
            if(x < Renderer::m_display->width && x >= 0 && y < Renderer::m_display->height && y >= 0)
            {
                double colorMix = (y-vert1->getY())/deltaY;
                Color *color = Color::lerp(col1,col2,colorMix);
                Renderer::putpixel(EMath::ceil(x),EMath::ceil(y),color->toUint32(Renderer::m_display->buffer->format));
            }
        }
    }

    //DEBUG
    Renderer::drawPoint(vert1);
    Renderer::drawPoint(vert2);
}
*/
void Renderer::putpixel(uint x, uint y, Uint32 color)
{
    Uint8 *i = (Uint8 *)Renderer::m_display->buffer->pixels + y * Renderer::m_display->surface->pitch + x * bytesPerPixel;
    *(Uint32 *)i = color;
}

void Renderer::rainbow()
{
    Color top(100,0,100);
    Color bottom(0,0,0);
    Color *color = NULL;
    double mix = 0;

    for(uint y = 0; y < Renderer::m_display->height; y++)
    {
        mix = EMath::normalize(y,0,Renderer::m_display->height);
        color = Color::lerp(&top,&bottom,mix);

        for(uint x = 0; x < Renderer::m_display->width; x++)
        {
            Renderer::putpixel(x,y,color->toUint32(Renderer::m_display->buffer->format));
        }
    }
}

void Renderer::drawSpan(Span *span, int y, Texture *tex)
{
    //prevent zero division error
    if(span->deltaX == 0)
        return;

    double coordMix = 0;
    double mixFactor = 1/span->deltaX;

    for(double x = span->minX; x < span->maxX; x++, coordMix+=mixFactor)
    {
        //casted to int to not compare signed and unsigned
        if(x < (int)Renderer::m_display->width && x >= 0 && y < (int)Renderer::m_display->height && y >= 0)
        {
            Vec3 *uv = Vec3::lerp(span->minCoord,span->maxCoord,coordMix);
            Renderer::putpixel(int(x),(int)y,*tex->getPixelAt(uv));
        }
    }
}

void Renderer::fillEdges(Edge *longEdge, Edge *shortEdge, Texture *tex)
{
    //if there is no vertical space, we have nothing fancy to do
    if(longEdge->deltaY == 0 || shortEdge->deltaY == 0)
    {
        return;
    }

    double longMix = (shortEdge->min->getY() - longEdge->min->getY()) / longEdge->deltaY;
    double shortMix = 0;
    double longFactor = 1/longEdge->deltaY;
    double shortFactor = 1/shortEdge->deltaY;

    // loop through the lines between the edges and draw spans
    for(double y = shortEdge->min->getY(); y < shortEdge->max->getY(); y++, longMix+=longFactor, shortMix+=shortFactor)
    {
        // create and draw span
        Span span(longEdge->min->getX()+(longEdge->deltaX*longMix),
                  shortEdge->min->getX()+(shortEdge->deltaX*shortMix),
                  Vec3::lerp(longEdge->min->texCoord,longEdge->max->texCoord,longMix),
                  Vec3::lerp(shortEdge->min->texCoord,shortEdge->max->texCoord,shortMix));
        Renderer::drawSpan(&span,y,tex);
    }
}

void Renderer::drawTriangle(Vertex *vert1, Vertex *vert2, Vertex *vert3, Texture *tex)
{

    //calculating normal
    //normalize (cross (B - A) (C - A))
    Vec3 n = Vec3::normalize(Vec3::cross(Vec3::subVectors(*vert2->location,*vert1->location),Vec3::subVectors(*vert3->location,*vert1->location)));
    /*
    std::cout << "DEBUG : tris\n";
    std::cout << Vec3(0,0,1).toString() << "\n";
    std::cout << n.toString() << "\n";
    std::cout << Vec3::dot(Vec3(0,0,1),n) << "\n";
    */
    //backface culling
    if(Vec3::dot(Vec3(0,0,1),n) >= 0)
        return;

    //if any point is too close, we don't render the triangle to avoid errors
    if(vert1->getZ()+Renderer::m_tanHalfFov <= 0 || vert2->getZ()+Renderer::m_tanHalfFov <= 0 || vert3->getZ()+Renderer::m_tanHalfFov <= 0)
        return;

    //screen space transformation after 3D projection
    Vertex *localVert1 = new Vertex(Vec3::toScreenSpace(Renderer::projectVector(vert1->location),Renderer::m_display->width),
                                    vert1->texCoord);
    Vertex *localVert2 = new Vertex(Vec3::toScreenSpace(Renderer::projectVector(vert2->location),Renderer::m_display->width),
                                    vert2->texCoord);
    Vertex *localVert3 = new Vertex(Vec3::toScreenSpace(Renderer::projectVector(vert3->location),Renderer::m_display->width),
                                    vert3->texCoord);


    //if all the points are outside the screen we don't render it
    if(((localVert1->getX() < 0 || localVert1->getX() > Renderer::m_display->width) || (localVert1->getY() < 0 || localVert1->getY() > Renderer::m_display->height)) &&
            ((localVert2->getX() < 0 || localVert2->getX() > Renderer::m_display->width) || (localVert2->getY() < 0 || localVert2->getY() > Renderer::m_display->height)) &&
            ((localVert3->getX() < 0 || localVert3->getX() > Renderer::m_display->width) || (localVert3->getY() < 0 || localVert3->getY() > Renderer::m_display->height)))
        return;

    //edges
    Edge edges[3] =
    {
        Edge(localVert1,localVert2),
        Edge(localVert2,localVert3),
        Edge(localVert3,localVert1),
    };

    //getting the longest edge index
    int shortEdgeIndex = 0;
    for(int i = 0; i < 3; i++)
    {
        if(edges[i].deltaY > edges[shortEdgeIndex].deltaY)
            shortEdgeIndex = i;
    }

    //filling the edges with the longest edge first, then the 2 other edges
    //(index+x)%vertPerTri makes sure we access the x other edge while staying in the valid indexies
    Renderer::fillEdges(&edges[shortEdgeIndex],&edges[(shortEdgeIndex+1)%vertPerTri],tex);
    Renderer::fillEdges(&edges[shortEdgeIndex],&edges[(shortEdgeIndex+2)%vertPerTri],tex);
}

void Renderer::drawMesh(Mesh *m)
{
    for(int i = 0; i < m->f.length(); i+=vertPerTri+coordPerTri+normalPerTri)
    {
        Renderer::drawTriangle( new Vertex(m->v[m->f[i  ]],m->vt[m->f[i+1]]),
                new Vertex(m->v[m->f[i+3]],m->vt[m->f[i+4]]),
                new Vertex(m->v[m->f[i+6]],m->vt[m->f[i+7]]),
                //m->vn[m->f[i+8]],
                m->tex);
    }
}

void Renderer::drawTex(Texture *tex)
{
    double uStep = Renderer::m_display->width/(double)tex->width;
    double vStep = Renderer::m_display->height/(double)tex->height;
    for(uint x = 0; x < Renderer::m_display->width; x++)
    {
        for(uint y = 0; y < Renderer::m_display->height; y++)
        {
            Renderer::putpixel(x,y,*(Uint32*) tex->rawPixel((int)(x/uStep),(int)(y/vStep)));
        }
    }
}

Vec3 *Renderer::projectVector(Vec3 *vec)
{
    /*
    Vec3 *vector = vec->copy();
    vector->z+= Renderer::m_tanHalfFov;

    double dz = Renderer::m_rotCos->x*(Renderer::m_rotCos->y*vector->z+Renderer::m_rotSin->y*(Renderer::m_rotSin->z*vector->x))-Renderer::m_rotSin->x*(Renderer::m_rotCos->z*vector->y-Renderer::m_rotSin->z*vector->x);
    if(dz == 0)
        return vec;

    double dx = Renderer::m_rotCos->y*(Renderer::m_rotSin->z*vector->y+Renderer::m_rotCos->z*vector->x)-Renderer::m_rotSin->y*vector->z;
    double dy = Renderer::m_rotSin->x*(Renderer::m_rotCos->y*vector->z+Renderer::m_rotSin->y*(Renderer::m_rotSin->z*vector->y+Renderer::m_rotCos->z*vector->x))+Renderer::m_rotCos->x*(Renderer::m_rotCos->z*vector->y-Renderer::m_rotSin->z*vector->x);

    double bx;
    double by;

    if(dx == 0)
        bx = dx;
    else
        bx = Renderer::m_tanHalfFov/dx;

    if(dy == 0)
        by = dy;
    else
        by = Renderer::m_tanHalfFov/dy;


    return new Vec3(bx,by,0);
    */
    Vec3 *projected = vec->copy();
    projected->z+=Renderer::m_tanHalfFov;
    if(projected->z != 0)
    {
        projected->x /= projected->z;
        projected->y /= projected->z;
    }

    return projected;
}

void Renderer::clearTarget()
{
    SDL_FillRect(Renderer::m_display->buffer,NULL,Renderer::m_backColor);
}

void Renderer::setBackColor(uint r, uint g, uint b)
{
    Renderer::m_backColor = SDL_MapRGB(Renderer::m_display->buffer->format,r,g,b);
}
