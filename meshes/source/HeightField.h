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

            void save();

        private:
            float pixelLength;
            float height;
            Point3 lowerLeft;

            std::shared_ptr<Image> image;

            Array<Point3> vertexList;
            Array<int>     indexList;

            const std::function<Color4(const Color4& src)> toGrayScale = [&](const Color4& src) {
                float intensity = 0.299f * src.r + 0.587f * src.g + 0.114f * src.b;
                return Color4(Color3(intensity), src.a);
            };

            void generate();
    };
};
