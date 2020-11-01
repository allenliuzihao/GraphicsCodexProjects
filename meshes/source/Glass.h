#pragma once

#include <G3D/G3D.h>
#include "Utilities.h"

namespace zl {
    class Glass {
    public:
        Glass();
        Glass(const Point3& bc, int ns);

        void setBottomCenter(const Point3& bc);
        void setNumberOfSlices(int nslices);

        void save(bool forceWrite = false);

    private:
        int numSlices;
        Point3 bottomCenter;

        Array<Point3> vertexList;
        Array<int> indexList;

        const String offFileLoc = "model/glass.off";
        const std::vector<Point3> contour = {
            Point3(0, 0, 0),        Point3(0.5, 0, 0),      Point3(0.5, 0.05, 0),
            Point3(0.35, 0.06, 0),  Point3(0.22, 0.08, 0),  Point3(0.1, 0.12, 0),
            Point3(0.05, 0.2, 0),   Point3(0.03, 0.32, 0),  Point3(0.028, 0.42, 0),
            Point3(0.027, 0.52, 0), Point3(0.05, 0.65, 0),  Point3(0.1, 0.79, 0),
            Point3(0.2, 0.86, 0),   Point3(0.33, 0.96, 0),  Point3(0.4, 1.01, 0),
            Point3(0.485, 1.11, 0), Point3(0.52, 1.22, 0),  Point3(0.54, 1.35, 0),
            Point3(0.56, 1.48, 0),  Point3(0.55, 1.6, 0),   Point3(0.53, 1.7, 0),
            Point3(0.5, 1.82, 0),   Point3(0.45, 1.91, 0),  Point3(0.385, 2, 0),
            Point3(0.32, 2.1, 0),   Point3(0.27, 2.1, 0),   Point3(0.31, 2.0, 0),   
            Point3(0.4, 1.91, 0),   Point3(0.45, 1.82, 0),  Point3(0.5, 1.7, 0),    
            Point3(0.51, 1.6, 0),   Point3(0.51, 1.48, 0),  Point3(0.5, 1.35, 0),   
            Point3(0.47, 1.22, 0),  Point3(0.435, 1.11, 0), Point3(0.35, 1.01, 0),  
            Point3(0.28, 0.96, 0),  Point3(0.15, 0.86, 0),  Point3(0.05, 0.79, 0),  
            Point3(0, 0.75, 0)
        };

        void generate();
    };
};
