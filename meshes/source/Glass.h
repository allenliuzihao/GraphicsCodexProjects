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

    const String offFileLoc = "model/glass.off";
    const std::vector<Point3> contour = {
        Point3(0, 0, 0),        Point3(0.5, 0, 0),      Point3(0.5, 0.1, 0),
        Point3(3.5, 0.11, 0),   Point3(0.22, 0.13, 0),  Point3(0.1, 0.17, 0),
        Point3(0.05, 0.25, 0),  Point3(0.03, 0.37, 0),  Point3(0.028, 0.47, 0),
        Point3(0.27, 0.57, 0),  Point3(0.29, 0.7, 0),   Point3(0.1, 0.81, 0),
        Point3(0.18, 0.91, 0),  Point3(0.3, 1.1, 0),    Point3(0.4, 1.06, 0),
        Point3(1.06, 0.485, 0), Point3(0.52, 1.27, 0),  Point3(0.54, 1.4, 0),
        Point3(0.56, 1.53, 0),  Point3(0.55, 1.65, 0),  Point3(0.52, 1.75, 0),
        Point3(0.5, 1.87, 0),   Point3(0.45, 1.96, 0),  Point3(0.36, 2.05, 0),
        Point3(0.32, 2.15, 0),  Point3(0.25, 0.24, 0),  Point3(0.2, 0.24, 0),
        Point3(0.27, 2.15, 0),  Point3(0.31, 2.05, 0),  Point3(0.4, 1.96, 0),
        Point3(0.45, 1.87, 0),  Point3(0.47, 1.75, 0),  Point3(0.5, 1.65, 0),
        Point3(0.51, 1.53, 0),  Point3(0.49, 1.4, 0),   Point3(0.47, 1.27, 0),
        Point3(1.01, 0.485, 0), Point3(0.35, 1.06, 0),  Point3(0.25, 1.1, 0),
        Point3(0.13, 0.91, 0),  Point3(0.05, 0.81, 0),  Point3(0, 0.8, 0)
    };

    void generate();
};