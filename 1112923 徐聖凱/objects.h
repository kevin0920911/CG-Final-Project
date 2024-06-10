#pragma once
#ifndef _OBJECT_H
#define _OBJECT_H
#include "mat_vec.h"

#define MAX_VERTICES 10000 // Max number of vertices (for each object)
#define MAX_POLYGONS 10000 // Max number of polygons (for each object)
typedef struct {
    float x, y, z;
}vertex_type;
struct Point {
    double x;
    double y;
    double z;
    Point(double x, double y, double z) {
        Point::x = x;
        Point::y = y;
        Point::z = z;
    }
};

typedef struct {
    unsigned short a, b, c;
}polygon_type;
typedef struct {
    float u, v;
}mapcoord_type;
typedef struct {

    char name[20]; // Name of the object

    int vertices_qty; // Number of vertices
    int polygons_qty; // Number of polygons

    vertex_type vertex[MAX_VERTICES]; // Array of vertices
    vertex_type normal[MAX_VERTICES]; // Array of the vertices' normals
    polygon_type polygon[MAX_POLYGONS]; // Array of polygons (numbers that point to the vertices' list)
    mapcoord_type mapcoord[MAX_VERTICES]; // Array of U,V coordinates for texture mapping

    int id_texture; // Number of the texture 

} obj_type, * obj_type_ptr;

extern obj_type object;
extern void ObjCalcNormals(obj_type_ptr p_object);
extern obj_type_ptr ObjLoad(char* p_object_name, char* p_texture_name);


#endif