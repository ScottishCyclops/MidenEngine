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
        Edge::minY = vert1;
        Edge::maxY = vert2;
    }
    else
    {
        Edge::minY = vert2;
        Edge::maxY = vert1;
    }

    Edge::deltaX = Edge::maxY->getX()-Edge::minY->getX();
    Edge::deltaY = Edge::maxY->getY()-Edge::minY->getY();
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
        Renderer::putpixel((int)vert->getX()%Renderer::m_display->width,
                           (int)vert->getY()%Renderer::m_display->height,
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

void Renderer::drawTriangle(Vertex *vert1, Vertex *vert2, Vertex *vert3)
{
    /*
    Edge edges[3] =
    {
        Edge(v0,v1),
        Edge(v1,v2),
        Edge(v2,v0),
    };

    //sorting edges from the smallest to tallest
    if(edges[0].getYMag() > edges[1].getYMag())
    {
        Edge tmp = edges[0];
        edges[0] = edges[1];
        edges[1] = tmp;
    }
    if(edges[1].getYMag() > edges[2].getYMag())
    {
        Edge tmp = edges[1];
        edges[1] = edges[2];
        edges[2] = tmp;

        if(edges[0].getYMag() > edges[1].getYMag())
        {
            Edge tmp = edges[0];
            edges[0] = edges[1];
            edges[1] = tmp;
        }
    }
    */

    /*
    Renderer::drawLine(vert1,vert2);
    Renderer::drawLine(vert2,vert3);
    Renderer::drawLine(vert3,vert1);
    */
    Edge e1(vert1,vert2);
    Edge e2(vert3,vert1);
    Renderer::fillEdges(&e1,&e2);
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

void Renderer::drawSpan(Span *span, uint y)
{
    if(span->deltaX == 0)
        return;

    double colorMix = 0;
    double mixFactor = 1/span->deltaX;

    for(double x = span->minX; x < span->maxX; x++, colorMix+=mixFactor)
    {
        Color *color = Color::lerp(span->minCol,span->maxCol,colorMix);
        Renderer::putpixel(EMath::ceil(x),EMath::ceil(y),color->toUint32(Renderer::m_display->buffer->format));
    }
}

void Renderer::fillEdges(Edge *edg1, Edge *edg2)
{
    //if there is no vertical space, we have nothing fancy to do
    if(edg1->deltaY == 0 || edg2->deltaY == 0)
    {
        Renderer::drawLine(edg1->minY,edg2->minY);
        return;
    }

    double factor1 = (edg2->minY->getY() - edg1->maxY->getY()) / edg1->deltaY;
    double factor2 = 0;
    double factorStep1 = 1/edg1->deltaY;
    double factorStep2 = 1/edg2->deltaY;

    // loop through the lines between the edges and draw spans
    for(double y = edg2->minY->getY(); y < edg2->maxY->getY(); y++, factor1+=factorStep1, factor2+=factorStep2)
    {
        // create and draw span
        Span s(edg1->minY->getX()+(edg1->deltaX*factor1),
               edg2->minY->getX()+(edg2->deltaX*factor2),
               Color::lerp(edg1->minY->color,edg1->maxY->color,factor1),
               Color::lerp(edg2->minY->color,edg2->maxY->color,factor2));
        Renderer::drawSpan(&s,y);
    }

    Renderer::drawLine(edg1->minY,edg1->maxY);
    Renderer::drawLine(edg2->minY,edg2->maxY);
}
