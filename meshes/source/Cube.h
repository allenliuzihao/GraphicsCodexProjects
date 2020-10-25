#pragma once

#include <fstream>
#include <iostream>

#include <G3D/G3D.h>

class Cube {
public:
    Cube();
    Cube(const Point3 &c, float length);

    void save();

private:
    Point3 center;
    float length;

    Array<Point3> vertexList;
    Array<int> indexList;

    void generate();
};

