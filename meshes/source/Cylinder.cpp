#include "Cylinder.h"

zl::Cylinder::Cylinder(): center(Point3(0, 0, 0)), radius(3.0f), height(5.0f) {
    generate();
}

zl::Cylinder::Cylinder(const Point3& c, float r, float h): center(c), radius(r), height(h) {
    generate();
}

void zl::Cylinder::save() {
    saveDataFile("model/cylinder.off", vertexList, indexList);
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
            indexList.append(2, sz - 1, sz - 2);
            indexList.append(2, 3, sz - 1);
            indexList.append(0, 2, sz - 2);
            indexList.append(3, 1, sz - 1);
            break;
        }

        mat = Matrix4::yawDegrees(total);
        
        top = (vertexList[vertexList.size() - 2] - center);
        bottom = (vertexList[vertexList.size() - 1] - center);

        newTop = mat * Vector4(top.x, top.y, top.z, 1);
        newBot = mat * Vector4(bottom.x, bottom.y, bottom.z, 1);

        top = newTop.xyz() + center;
        bottom = newBot.xyz() + center;

        vertexList.append(top, bottom);

        sz = vertexList.size();
        indexList.append(sz - 4, sz - 2, sz - 3);
        indexList.append(sz - 2, sz - 1, sz - 3);
        indexList.append(0, sz - 2, sz - 4);
        indexList.append(sz - 1, 1, sz - 3);
    }
}