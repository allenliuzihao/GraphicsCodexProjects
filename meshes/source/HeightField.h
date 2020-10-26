#pragma once

#include <G3D/G3D.h>

class HeightField {
public:
    HeightField(const std::string& fn);
    HeightField(const Point3& ll, float pl, const std::string &fn);

private:
    float pixelLength;
    Point3 lowerLeft;

    std::shared_ptr<Image> image;

    const std::function<Color4(const Color4& src)> lambda = [&](const Color4& src) {
        float intensity = 0.299f * src.r + 0.587f * src.g + 0.114f * src.b;
        return Color4(Color3(intensity), src.a);
    };
};