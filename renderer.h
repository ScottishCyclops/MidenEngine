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
#define coordPerTri 3

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
    Vec3 *minCoord;
    Vec3 *maxCoord;

    double deltaX;
    double deltaU;
    double deltaV;

    Span(double x1, double x2, Vec3 *coord1, Vec3 *coord2);
};

// Renderer //

class Renderer
{
private:
    Display *m_display;
    Uint32 m_backColor;
    double m_fov;
    double m_tanHalfFov;
    Vec3 *m_rotation;
    Vec3 *m_rotSin;
    Vec3 *m_rotCos;

    void putpixel(uint x, uint y, Uint32 color);
    void drawSpan(Span *span, int y, Texture *tex);
    void fillEdges(Edge *shortEdge, Edge *longEdge, Texture *tex);
    Vec3 *projectVector(Vec3 *vec);
    void updateRotation();

public:
    Renderer(Display *display, double fov);
    //void drawPoint(Vertex *vert);
    //void drawLine(Vertex *vert1, Vertex *vert2);
    void drawTriangle(Vertex *vert1, Vertex *vert2, Vertex *vert3, Texture *tex);
    void drawMesh(Mesh *m);
    void rainbow();
    void drawTex(Texture *tex);
    void clearTarget();
    void setBackColor(uint r, uint g, uint b);
};

#endif // RENDERER_H
