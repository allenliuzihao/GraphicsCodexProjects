#pragma once

#include <G3D/G3D.h>

class SphereCubes {
public:
    SphereCubes();
    SphereCubes(const G3D::Point3& c, float r, int los, int las);

    void saveAny(const std::string& fileLoc);

private:
    float radius;
    G3D::Point3 center;

    int longitudeSteps;
    int latitudeSteps;

    Any any = Any(Any::TABLE);

    void generate();

    G3D::Point3 evaluate(float theta, float phi);
};