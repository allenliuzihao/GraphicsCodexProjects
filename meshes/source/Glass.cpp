#include "Glass.h"

using namespace zl;

Glass::Glass(): bottomCenter(0, 0, 0), numSlices(10) {}

Glass::Glass(const Point3& bc, int ns) : bottomCenter(bc) {
    assert(ns > 2 && "at least 2 slices is required.");

    numSlices = ns;
}

void Glass::setBottomCenter(const Point3& bc) {
    this->bottomCenter = bc;
}

void Glass::setNumberOfSlices(int nslices) {
    this->numSlices = nslices;
}

void Glass::save(bool forceWrite) {
    if (fileExists(offFileLoc) && !forceWrite) {
        return;
    }

    generate();
    saveDataFile(offFileLoc.c_str(), vertexList, indexList);
}

void Glass::generate() {
    vertexList.clear();
    indexList.clear();

    Point3 bc = contour[0] + bottomCenter;
    Point3 tc = contour[contour.size() - 1] + bottomCenter;
    vertexList.append(tc, bc);

    float degreesPerSlice = 360.0f / numSlices;
    float totalDegrees = 0.0f;

    int sz, idx;
    Point3 pt;
    Vector4 newPt;
    Matrix4 mat;

    vertexList.append(contour[1] + bottomCenter);
    while (totalDegrees < 360.0f) {
        totalDegrees += degreesPerSlice;
        if (360.f - totalDegrees <= 1.0f) {
            sz = (int) vertexList.size();
            indexList.append(sz - 1, 1, 2);
            break;
        }

        mat = Matrix4::yawDegrees(totalDegrees);
        newPt = mat * Vector4(contour[1], 1);
        pt = newPt.xyz() + bottomCenter;

        vertexList.append(pt);
        sz = (int) vertexList.size();

        indexList.append(sz - 2, 1, sz - 1);
    }

    for (int iter = 2; iter < contour.size() - 1; ++iter) {
        totalDegrees = 0.0f;
        vertexList.append(contour[iter] + bottomCenter);
        idx = vertexList.size() - 1;
        while (totalDegrees < 360.0f) {
            totalDegrees += degreesPerSlice;
            if (360.f - totalDegrees <= 1.0f) {
                sz = (int)vertexList.size();
                indexList.append(idx, sz - 1, idx - numSlices);
                indexList.append(sz - 1, sz - 1 - numSlices, idx - numSlices);
                break;
            }

            mat = Matrix4::yawDegrees(totalDegrees);
            newPt = mat * Vector4(contour[iter], 1);
            pt = newPt.xyz() + bottomCenter;

            vertexList.append(pt);
            sz = (int)vertexList.size();

            indexList.append(sz - 1, sz - 2, sz - 2 - numSlices);
            indexList.append(sz - 1, sz - 2 - numSlices, sz - 1 - numSlices);
        }
    }
    
    totalDegrees = 0.0f;
    vertexList.append(contour[contour.size() - 2] + bottomCenter);
    idx = vertexList.size() - 1;
    while (totalDegrees < 360.0f) {
        totalDegrees += degreesPerSlice;
        if (360.f - totalDegrees <= 1.0f) {
            sz = (int) vertexList.size();
            indexList.append(0, sz - 1, idx);
            break;
        }

        mat = Matrix4::yawDegrees(totalDegrees);
        newPt = mat * Vector4(contour[contour.size() - 2], 1);
        pt = newPt.xyz() + bottomCenter;

        vertexList.append(pt);
        sz = (int)vertexList.size();

        indexList.append(0, sz - 2, sz - 1);
    }
}