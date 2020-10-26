#include "HeightField.h"

HeightField::HeightField(const std::string& fn): 
    lowerLeft(Point3(0.0f, 0.0f, 0.0f)), pixelLength(0.1f), image(G3D::Image::fromFile(String(fn))) {
    image->forEachPixel<Color4>(lambda);
}

HeightField::HeightField(const Point3& ll, float pl, const std::string& fn): lowerLeft(ll), image(G3D::Image::fromFile(String(fn))) {
    assert(pl >= 0.0f && "pixel length should be > 0.0f.");
    pixelLength = pl;
    image->forEachPixel<Color4>(lambda);
}


