#pragma once

#include <fstream>
#include <iostream>

#include <G3D/G3D.h>

#include "Utilities.h"

namespace zl {

    class Cube {
    public:
        Cube();
        Cube(const Point3& c, float length);

        void setCenter(const Point3 &c);
        void setLength(float l);
        void save();

    private:
        Point3 center;
        float length;

        Array<Point3> vertexList;
        Array<int> indexList;

        void generate();
    };
}
