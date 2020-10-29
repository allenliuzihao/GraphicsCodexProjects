#include "Glass.h"

Glass::Glass(): bottomCenter(0, 0, 0), numSlices(10) {}

Glass::Glass(const Point3& bc, int ns) : bottomCenter(bc) {
    assert(ns > 2 && "at least 2 slices is required.");

    numSlices = ns;
}

void Glass::generate() {
    vertexList.clear();
    indexList.clear();



}