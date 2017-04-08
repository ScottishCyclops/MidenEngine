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

Mesh::Mesh(QList<Vec3*> v, QList<int> f)
{
    Mesh::v = v;
    Mesh::f = f;
    Mesh::tex = NULL;
}

Mesh::Mesh(QList<Vec3*> v, QList<Vec3 *> vt, QList<int> f, Texture *tex)
{
    Mesh::v = v;
    Mesh::vt = vt;
    Mesh::f = f;
    Mesh::tex = tex;
}

void Mesh::translate(double x, double y, double z)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->translate(x,y,z);
    }
}

void Mesh::rotateX(double angle, Vec3 origin)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->rotateX(angle,origin);
    }
}

void Mesh::rotateY(double angle, Vec3 origin)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->rotateY(angle,origin);
    }
}

void Mesh::rotateZ(double angle, Vec3 origin)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->rotateZ(angle,origin);
    }
}

void Mesh::scale(double x, double y, double z, Vec3 origin)
{
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        Mesh::v[i]->scale(x,y,z,origin);
    }
}

Vec3 Mesh::getCenter()
{
    Vec3 total(0,0,0);
    for(int i = 0; i < Mesh::v.length(); i++)
    {
        total = Vec3::addVectors(total,*Mesh::v[i]);
    }

    return Vec3::divScalar(total,Mesh::v.length());
}

Mesh *Mesh::importMesh(string objPath, Texture *tex)
{
    ifstream objFile(objPath);

    //the data we are going to get
    //vertex
    QList<Vec3 *>v;
    //vertex texture coord
    QList<Vec3 *>vt;
    //faces
    QList<int> f;

    for(string line; getline(objFile, line);)
    {
        //transforming to QString to have better comparaison fonctions
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
                QStringList values = pos[i].split("/");
                //-1 to get a 0 indexed list, not 1
                //face index
                f.append(values[0].toInt()-1);
                //vertex texture index
                f.append(values[1].toInt()-1);
                //we have a total of 6 componants per face
            }
        }
    }
    return new Mesh(v,vt,f,tex);
}
