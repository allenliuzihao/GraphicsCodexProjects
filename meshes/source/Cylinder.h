#pragma once

#include <G3D/G3D.h>

#include "Utilities.h"

namespace zl {
    class Cylinder {
        public:
            Cylinder();
            Cylinder(const Point3& c, float r, float h);

            void setCenter(const Point3 &c);
            void setHeight(float h);
            void setRadius(float r);
            void set(float h, float r);

            void save(bool forceWrite = false);

        private:
            const float degreesPerQuad = 5.0f;

            float radius;
            float height;

            Point3 center;

            Array<Point3> vertexList;
            Array<int> indexList;

            const String offFileLoc = "model/cylinder.off";

            void generate();
    };
};