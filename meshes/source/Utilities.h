#pragma once

#include <G3D/G3D.h>

#include <fstream>
#include <iostream>

static inline void saveDataFile(const std::string& fileLoc, const Array<Point3> &vertexList, const Array<int> &indexList) {
    std::ofstream file(fileLoc);
    int faces = (indexList.size() / 3);

    file << "OFF\n";
    file << vertexList.size() << " " << faces << " " << 18 << "\n";     // the last value is unused.
    file << "\n";

    for (const auto& point : vertexList) {
        file << point.x << " " << point.y << " " << point.z << "\n";
    }

    file << "\n";

    for (int i = 0; i < faces; ++i) {
        file << 3 << " " << indexList[3 * i] << " " << indexList[3 * i + 1] << " " << indexList[3 * i + 2] << "\n";
    }

    file.close();
}