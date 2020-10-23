#pragma once

#include <G3D/G3D.h>

class Staircase{
public:
    Staircase();
    Staircase(const G3D::Point3& pt, float r, float sl, float sw, float sh, int ns);

    void saveAny(const std::string& fileLoc);

private:
    G3D::Point3 center;

    float rotateDegree;

    float stairLength;
    float stairWidth;
    float stairHeight;

    int numSteps;

    Any any = Any(Any::TABLE);

    void generate();
};