#include "Cube.h"

Cube::Cube() : center(Point3(0, 0, 0)), length(1) {
    generate();
}

Cube::Cube(const Point3& c, float l): center(c), length(l) {
    generate();
}

void Cube::save() {
    std::ofstream file("model/cube.off");

    file << "OFF\n";
    file << 8 << " " << 12 << " " << 18 << "\n";
    file << "\n";

    for (const auto& point : vertexList) {
        file << point.x << " " << point.y << " " << point.z << "\n";
    }

    file << "\n";

    for (int i = 0; i < 12; ++i) {
        file << 3 << " " << indexList[3 * i] << " " << indexList[3 * i + 1] << " " << indexList[3 * i + 2] << "\n";
    }

    file.close();
}

void Cube::generate() {
    vertexList.clear();
    indexList.clear();

    vertexList.append(Point3(length / 2.0f, -length/2.0f, length/2.0f));
    vertexList.append(Point3(length / 2.0f, length / 2.0f, length / 2.0f));
    vertexList.append(Point3(length / 2.0f, length / 2.0f, -length / 2.0f));
    vertexList.append(Point3(length / 2.0f, -length / 2.0f, -length / 2.0f));
    
    vertexList.append(Point3(-length / 2.0f, -length / 2.0f, length / 2.0f));
    vertexList.append(Point3(-length / 2.0f, length / 2.0f, length / 2.0f));
    vertexList.append(Point3(-length / 2.0f, length / 2.0f, -length / 2.0f));
    vertexList.append(Point3(-length / 2.0f, -length / 2.0f, -length / 2.0f));

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