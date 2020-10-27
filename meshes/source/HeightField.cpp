#include "HeightField.h"

using namespace zl;

HeightField::HeightField(): lowerLeft(Point3(0.0f, 0.0f, 0.0f)), 
                            height(0.5f), 
                            pixelLength(0.01f),
                            image(G3D::Image::fromFile("image/height_field.png")) {
    image->forEachPixel<Color4>(toGrayScale);
}

HeightField::HeightField(const Point3& ll, float h, float pl, const String &fn): lowerLeft(ll), image(G3D::Image::fromFile(fn)) {
    assert(pl >= 0.0f && "pixel length should be > 0.0f.");
    assert(h >= 0.0f && "height should be > 0.0f.");

    pixelLength = pl;
    height = h;
    image->forEachPixel<Color4>(toGrayScale);   
}

void HeightField::setLowerLeft(const Point3 & ll) {
    lowerLeft = ll;
}

void HeightField::setHeight(float h) {
    height = h;
}

void HeightField::setPixelLength(float p) {
    pixelLength = p;
}

void HeightField::setImage(const String &fn) {
    image = G3D::Image::fromFile(fn);
    image->forEachPixel<Color4>(toGrayScale);
}

void HeightField::save(bool forceWrite) {
    generate();
    saveDataFile("model/heightfield.off", vertexList, indexList, forceWrite);
}

void HeightField::generate() {
    vertexList.clear();
    indexList.clear();

    Point3 v;
    Color1 c;
    int sz;
    int W = image->width(), H = image->height();
    float currW = 0.0f, currH = 0.0f;

    for (int w = 0; w < W; ++w) {
        currW = static_cast<float>(w) * pixelLength;

        image->get(Point2int32(w, 0), c);
        v = Point3(currW, c.value * height, currH) + lowerLeft;
        vertexList.append(v);
    }

    for (int h = 1; h < H; ++h) {
        currW = 0.0f;
        currH = static_cast<float>(h) * pixelLength;

        image->get(Point2int32(0, h), c);
        v = Point3(currW, c.value * height, currH) + lowerLeft;
        vertexList.append(v);

        for (int w = 1; w < W; ++w) {
            currW = static_cast<float>(w) * pixelLength;

            image->get(Point2int32(w, h), c);
            v = Point3(currW, c.value * height, currH) + lowerLeft;
            vertexList.append(v);

            sz = vertexList.size();

            indexList.append(sz - 1 - (W + 1), sz - 2, sz - 1);
            indexList.append(sz - 1 - (W + 1), sz - 1, sz - 1 - W);
        }
    }
}