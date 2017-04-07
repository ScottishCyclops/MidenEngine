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

#include "mesh.h"

Mesh::Mesh(QList<Vertex*> v, QList<int> t)
{
    Mesh::v = v;
    Mesh::t = t;
    Mesh::tex = NULL;
}

Mesh::Mesh(QList<Vertex*> v, QList<int> t, Texture *tex)
{
    Mesh::v = v;
    Mesh::t = t;
    Mesh::tex = tex;
}

void Mesh::translate(double x, double y, double z)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->location->translate(x,y,z);
    }
}

void Mesh::rotateX(double angle, Vec3 origin)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->location->rotateX(angle,origin);
    }
}

void Mesh::rotateY(double angle, Vec3 origin)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->location->rotateY(angle,origin);
    }
}

void Mesh::rotateZ(double angle, Vec3 origin)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->location->rotateZ(angle,origin);
    }
}

void Mesh::scale(double x, double y, double z, Vec3 origin)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->location->scale(x,y,z,origin);
    }
}

Vec3 Mesh::getCenter()
{
    Vec3 total(0,0,0);
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        total = Vec3::addVectors(total,*Mesh::v[i]->location);
    }

    return Vec3::divScalar(total,Mesh::v.length());
}

Mesh *Mesh::importMesh(string objPath, Texture *tex)
{
/*
    """reads an .obj file and returns a mesh build from it
    For now, assumes that the .obj contains only one mesh
    and the faces are all triangles."""
    fileName = "./resources/"+fileName+".obj"

    #r -> read mode
    with open(fileName, "r") as f:
        content = f.read()
    f.close()
    lines = content.splitlines()

    '''Vectors'''
    vs = []
    for line in lines:
        if line.startswith("v"):
            #we found a vertex
            v = line[2:].split(" ")
            vs.append(Vector(float(v[0]), float(v[1]), float(v[2])))

    '''Triangles'''
    ts = []
    for line in lines:
        if line.startswith("f"):
            #we found a face
            #we assume it's a triangle
            #TODO: triangulate faces
            f = line[2:].split(" ")
            #-1 because our vectors list first index is 0, not 1
            ts.append(int(f[0])-1)
            ts.append(int(f[1])-1)
            ts.append(int(f[2])-1)

    return Mesh(np.array(vs),np.array(ts,np.int16))
    */
    ifstream objFile(objPath);

    //the data we are going to get
    //vectors
    QList<Vec3 *>v;
    //texture coordinates
    QList<Vec3 *>vt;
    //face index
    QList<int> f;

    for(string line; getline(objFile, line);)
    {
        QString qline = line.c_str();

        //if we found a vertex
        if(qline.startsWith("v "))
        {
            QStringList pos = qline.split(" ");
            v.append(new Vec3(pos[1].toDouble(),pos[2].toDouble(),pos[3].toDouble()));
        }
        //we found a texture coord
        else if(qline.startsWith("vt "))
        {
            QStringList pos = qline.split(" ");
            vt.append(new Vec3(pos[1].toDouble(),pos[2].toDouble(),0));
        }
        //we found a face
        else if(qline.startsWith("f "))
        {
            QStringList pos = qline.split(" ");
            //we skip the first element because it is the "f"
            for(int i = 1; i < pos.length(); i++)
            {
                //-1 to get a 0 indexed list, not 1
                f.append(pos[i].split("/")[0].toInt()-1);
            }
        }
    }
    QList<Vertex *> vertexs;
    //generating vertexs
    for(int i = 0; i < v.length(); i++)
    {
        vertexs.append(new Vertex(v[i],vt[i]));
    }
    return new Mesh(vertexs,f,tex);
}
