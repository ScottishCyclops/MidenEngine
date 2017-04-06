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

Span::Span(double x1, double x2, Color *col1, Color *col2)
{
    //sort xs by minimum
    if(x1 < x2)
    {
        Span::minX = x1;
        Span::maxX = x2;
        Span::minCol = col1;
        Span::maxCol = col2;
    }
    else
    {
        Span::minX = x2;
        Span::maxX = x1;
        Span::minCol = col2;
        Span::maxCol = col1;
    }

    Span::deltaX = Span::maxX-Span::minX;
}

// Renderer //

Renderer::Renderer(Display *display)
{
    Renderer::m_display = display;
}

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

void Renderer::drawSpan(Span *span, int y)
{
    if(span->deltaX == 0)
        return;

    double colorMix = 0;
    double mixFactor = 1/span->deltaX;

    for(double x = span->minX; x < span->maxX; x++, colorMix+=mixFactor)
    {
        //casted to int to not compare signed and unsigned
        if(x < (int)Renderer::m_display->width && x >= 0 && y < (int)Renderer::m_display->height && y >= 0)
        {
            Color *color = Color::lerp(span->minCol,span->maxCol,colorMix);
            Renderer::putpixel(EMath::ceil(x),EMath::ceil(y),color->toUint32(Renderer::m_display->buffer->format));
        }
    }
}

void Renderer::fillEdges(Edge *longEdge, Edge *shortEdge)
{
    //if there is no vertical space, we have nothing fancy to do
    if(longEdge->deltaY == 0 || shortEdge->deltaY == 0)
    {
        Renderer::drawLine(longEdge->min,shortEdge->min);
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
                  Color::lerp(longEdge->min->color, longEdge->max->color, longMix),
                  Color::lerp(shortEdge->min->color, shortEdge->max->color, shortMix));
        Renderer::drawSpan(&span,y);
    }
}

void Renderer::drawTriangle(Vertex *vert1, Vertex *vert2, Vertex *vert3)
{

    //screen space transformation
    Vertex *localVert1 = new Vertex(Vec3::toScreenSpace(vert1->location,Renderer::m_display->width),
                                    vert1->color->copy(),
                                    vert1->texCoord->copy());
    Vertex *localVert2 = new Vertex(Vec3::toScreenSpace(vert2->location,Renderer::m_display->width),
                                    vert2->color->copy(),
                                    vert2->texCoord->copy());
    Vertex *localVert3 = new Vertex(Vec3::toScreenSpace(vert3->location,Renderer::m_display->width),
                                    vert3->color->copy(),
                                    vert3->texCoord->copy());

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
    Renderer::fillEdges(&edges[shortEdgeIndex],&edges[(shortEdgeIndex+1)%vertPerTri]);
    Renderer::fillEdges(&edges[shortEdgeIndex],&edges[(shortEdgeIndex+2)%vertPerTri]);
}

void Renderer::drawMesh(Mesh *m)
{
    for(int i = 0; i < m->t.length(); i+=vertPerTri)
    {
        Renderer::drawTriangle(m->v[m->t[i]], m->v[m->t[i+1]], m->v[m->t[i+2]]);
    }
}
