#include "Cylinder.h"

zl::Cylinder::Cylinder(): center(Point3(0, 0, 0)), radius(3.0f), height(5.0f) {}

zl::Cylinder::Cylinder(const Point3& c, float r, float h): center(c), radius(r), height(h) {}

void zl::Cylinder::save() {
    generate();
    saveDataFile("model/cylinder.off", vertexList, indexList);
}

void zl::Cylinder::setRadius(float r) {
    radius = r;
}

void zl::Cylinder::setHeight(float h) {
    height = h;
}

void zl::Cylinder::setCenter(const Point3& c) {
    center = c;
}

void zl::Cylinder::set(float h, float r) {
    height = h;
    radius = r;
}

void zl::Cylinder::generate() {
    vertexList.clear();
    indexList.clear();

    Point3 bottomCenter = Point3(0, -height / 2.0f, 0) + center;
    Point3 topCenter = Point3(0, height / 2.0f, 0) + center;
    vertexList.append(topCenter, bottomCenter);

    Point3 initialTop = Point3(radius, height / 2.0f, 0) + center;
    Point3 initialBottom = Point3(radius, -height / 2.0f, 0) + center;    
    vertexList.append(initialTop, initialBottom);

    float total = 0.0f;
    
    int sz;
    Matrix4 mat;
    Point3 top, bottom;
    Vector4 newTop, newBot;
    
    while (total < 360.0f) {
        total += degreesPerQuad;
        if (total >= 360) {
            sz = vertexList.size();
            indexList.append(sz - 2, 3, 2);
            indexList.append(sz - 2, sz - 1, 3);
            indexList.append(2, 0, sz - 2);
            indexList.append(sz - 1, 1, 3);
            break;
        }

        mat = Matrix4::yawDegrees(total);
        
        newTop = mat * Vector4(radius, height / 2.0f, 0, 1);
        newBot = mat * Vector4(radius, -height / 2.0f, 0, 1);

        top = newTop.xyz() + center;
        bottom = newBot.xyz() + center;

        vertexList.append(top, bottom);

        sz = vertexList.size();
        indexList.append(sz - 4, sz - 1, sz - 2);
        indexList.append(sz - 4, sz - 3, sz - 1);
        indexList.append(sz - 2, 0, sz - 4);
        indexList.append(sz - 3, 1, sz - 1);
    }
}