#include "Cube.h"

using namespace zl;

Cube::Cube() : center(Point3(0, 0, 0)), length(1) {
    generate();
}

Cube::Cube(const Point3& c, float l): center(c), length(l) {
    generate();
}

void Cube::setCenter(const Point3& c) {
    center = c;
    generate();
}

void Cube::setLength(float l) {
    length = l;
    generate();
}

void Cube::save() {
    saveDataFile("model/cube.off", vertexList, indexList);
}

void Cube::generate() {
    vertexList.clear();
    indexList.clear();

    vertexList.append(Point3(length / 2.0f, -length/2.0f, length/2.0f) + center);
    vertexList.append(Point3(length / 2.0f, length / 2.0f, length / 2.0f) + center);
    vertexList.append(Point3(length / 2.0f, length / 2.0f, -length / 2.0f) + center);
    vertexList.append(Point3(length / 2.0f, -length / 2.0f, -length / 2.0f) + center);
    
    vertexList.append(Point3(-length / 2.0f, -length / 2.0f, length / 2.0f) + center);
    vertexList.append(Point3(-length / 2.0f, length / 2.0f, length / 2.0f) + center);
    vertexList.append(Point3(-length / 2.0f, length / 2.0f, -length / 2.0f) + center);
    vertexList.append(Point3(-length / 2.0f, -length / 2.0f, -length / 2.0f) + center);

    indexList.append(2, 1, 0);
    indexList.append(3, 2, 0);
    indexList.append(5, 6, 7);
    indexList.append(4, 5, 7);
    indexList.append(1, 5, 0);
    indexList.append(0, 5, 4);
    indexList.append(6, 2, 3);
    indexList.append(7, 6, 3);
    indexList.append(2, 6, 1);
    indexList.append(6, 5, 1);
    indexList.append(4, 7, 0);
    indexList.append(7, 3, 0);
}