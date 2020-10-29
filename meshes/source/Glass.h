#pragma once

#include <G3D/G3D.h>

class Glass {
public:
    Glass();
    Glass(const Point3& bc, int ns);

    void save(bool forceWrite = false);

private:
    int numSlices;
    Point3 bottomCenter;
    
    Array<Point3> vertexList;
    Array<int> indexList;

    const String offFileLoc = "model/cylinder.off";
    const std::vector<Point3> contour = {
        
    };

    void generate();
};