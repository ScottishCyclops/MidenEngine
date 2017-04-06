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

#ifndef RENDERER_H
#define RENDERER_H

#define vertPerTri 3

#include "includes.h"

#include "display.h"
#include "vertex.h"
#include "mesh.h"

// Edge //

class Edge
{
public:
    double deltaX;
    double deltaY;
    Vertex *min;
    Vertex *max;
    Edge(Vertex *vert1, Vertex *vert2);
};

// Span //

class Span
{
public:
    double minX;
    double maxX;
    double deltaX;
    Color *minCol;
    Color *maxCol;
    Span(double x1, double x2, Color *col1, Color *col2);
};

// Renderer //

class Renderer
{
private:
    Display *m_display;
    void putpixel(uint x, uint y, Uint32 color);
    void drawSpan(Span *span, int y);
    void fillEdges(Edge *shortEdge, Edge *longEdge);
public:
    Renderer(Display *display);
    void drawPoint(Vertex *vert);
    void drawLine(Vertex *vert1, Vertex *vert2);
    void drawTriangle(Vertex *vert1, Vertex *vert2, Vertex *vert3);
    void drawMesh(Mesh *m);
    void rainbow();
};

#endif // RENDERER_H
