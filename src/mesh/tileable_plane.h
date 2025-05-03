#pragma once

#include "../../helper/trianglemesh.h"

class TileablePlane : public TriangleMesh {
public:
    TileablePlane(float size = 10.0f, int segments = 64, float tileFactor = 5.0f);

private:
    void GenerateMeshData(float size, int segments, float tileFactor);
};