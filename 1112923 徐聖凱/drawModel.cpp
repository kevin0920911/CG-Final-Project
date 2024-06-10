#include <GL/glut.h>
#include "drawModel.h"
void drawModel(obj_type_ptr object) {
    const int p_qty = object->polygons_qty;
    if (object->id_texture != -1)
    {
        glBindTexture(GL_TEXTURE_2D, object->id_texture); // We set the active texture 
        glEnable(GL_TEXTURE_2D); // Texture mapping ON
    }
    else
        glDisable(GL_TEXTURE_2D); // Texture mapping OFF
    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    for (int j = 0; j < p_qty; j++)
    {
        //----------------- FIRST VERTEX -----------------
        //Normal coordinates of the first vertex
        glNormal3f(object->normal[object->polygon[j].a].x,
            object->normal[object->polygon[j].a].y,
            object->normal[object->polygon[j].a].z);
        // Texture coordinates of the first vertex
        glTexCoord2f(object->mapcoord[object->polygon[j].a].u,
            object->mapcoord[object->polygon[j].a].v);
        // Coordinates of the first vertex
        glVertex3f(object->vertex[object->polygon[j].a].x,
            object->vertex[object->polygon[j].a].y,
            object->vertex[object->polygon[j].a].z);

        //----------------- SECOND VERTEX -----------------
        //Normal coordinates of the second vertex
        glNormal3f(object->normal[object->polygon[j].b].x,
            object->normal[object->polygon[j].b].y,
            object->normal[object->polygon[j].b].z);
        // Texture coordinates of the second vertex
        glTexCoord2f(object->mapcoord[object->polygon[j].b].u,
            object->mapcoord[object->polygon[j].b].v);
        // Coordinates of the second vertex
        glVertex3f(object->vertex[object->polygon[j].b].x,
            object->vertex[object->polygon[j].b].y,
            object->vertex[object->polygon[j].b].z);

        //----------------- THIRD VERTEX -----------------
        //Normal coordinates of the third vertex
        glNormal3f(object->normal[object->polygon[j].c].x,
            object->normal[object->polygon[j].c].y,
            object->normal[object->polygon[j].c].z);
        // Texture coordinates of the third vertex
        glTexCoord2f(object->mapcoord[object->polygon[j].c].u,
            object->mapcoord[object->polygon[j].c].v);
        // Coordinates of the Third vertex
        glVertex3f(object->vertex[object->polygon[j].c].x,
            object->vertex[object->polygon[j].c].y,
            object->vertex[object->polygon[j].c].z);
    }
    glEnd();
}