#pragma once

#include <G3D/G3D.h>

#include "Utilities.h"

namespace zl {
    class Cylinder {
        public:
            Cylinder();
            Cylinder(const Point3& c, float r, float h);

            void save();

        private:
            const float degreesPerQuad = 17.0f;

            float radius;
            float height;

            Point3 center;

            Array<Point3> vertexList;
            Array<int> indexList;

            void generate();
    };
};