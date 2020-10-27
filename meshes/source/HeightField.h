#pragma once

#include <G3D/G3D.h>

#include "Utilities.h"

namespace zl {
    class HeightField {
        public:
            HeightField();
            HeightField(const Point3& ll, float h, float pl, const String& fn);

            void setLowerLeft(const Point3& ll);
            void setHeight(float h);
            void setPixelLength(float p);
            void setImage(const String &fn);

            void save(bool forceWrite = false);

        private:
            float pixelLength;
            float height;
            Point3 lowerLeft;

            String imageLoc;

            Array<Point3> vertexList;
            Array<int>     indexList;

            const String offFileLoc = "model/heightfield.off";

            const std::function<Color1(const Color4& src)> getIntensity = [&](const Color4& src) {
                float intensity = 0.299f * src.r + 0.587f * src.g + 0.114f * src.b;
                return Color1(intensity);
            };

            void generate();
    };
};
