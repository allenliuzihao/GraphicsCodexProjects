#include "HeightField.h"

using namespace zl;

HeightField::HeightField(): lowerLeft(Point3(0.0f, 0.0f, 0.0f)), 
                            height(0.5f), 
                            pixelLength(0.01f),
                            imageLoc("image/height_field.png") {}

HeightField::HeightField(const Point3& ll, float h, float pl, const String &fn): lowerLeft(ll) {
    assert(pl > 0.0f && "pixel length should be > 0.0f.");
    assert(h > 0.0f && "height should be > 0.0f.");
    assert(!fn.empty() && "image location should be non-empty.");

    pixelLength = pl;
    height = h;
    imageLoc = fn;
}

void HeightField::setLowerLeft(const Point3 & ll) {
    lowerLeft = ll;
}

void HeightField::setHeight(float h) {
    assert(h > 0.0f && "height should be > 0.0f.");
    height = h;
}

void HeightField::setPixelLength(float p) {
    assert(p > 0.0f && "pixel length should be > 0.0f.");
    pixelLength = p;
}

void HeightField::setImage(const String &fn) {
    assert(!fn.empty() && "image location should be non-empty.");
    imageLoc = fn;
}

void HeightField::save(bool forceWrite) {
    if (fileExists(zipFileLoc) && !forceWrite) {
        return;
    }

    generate();
    saveDataFile(offFileLoc.c_str(), vertexList, indexList);
}

void HeightField::generate() {
    vertexList.clear();
    indexList.clear();

    std::shared_ptr<Image> image = G3D::Image::fromFile(imageLoc);

    Point3 v;
    Color1 it;
    Color4 c;
    int sz;
    int W = image->width(), H = image->height();
    float currW = 0.0f, currH = 0.0f;

    for (int w = 0; w < W; ++w) {
        currW = static_cast<float>(w) * pixelLength;

        image->get(Point2int32(w, 0), c);
        it = getIntensity(c);
        v = Point3(currW, it.value * height, currH) + lowerLeft;
        vertexList.append(v);
    }

    for (int h = 1; h < H; ++h) {
        currW = 0.0f;
        currH = static_cast<float>(h) * pixelLength;

        image->get(Point2int32(0, h), c);
        it = getIntensity(c);
        v = Point3(currW, it.value * height, currH) + lowerLeft;
        vertexList.append(v);

        for (int w = 1; w < W; ++w) {
            currW = static_cast<float>(w) * pixelLength;

            image->get(Point2int32(w, h), c);
            it = getIntensity(c);
            v = Point3(currW, it.value * height, currH) + lowerLeft;
            vertexList.append(v);

            sz = vertexList.size();

            indexList.append(sz - 1 - (W + 1), sz - 2, sz - 1);
            indexList.append(sz - 1 - (W + 1), sz - 1, sz - 1 - W);
        }
    }
}