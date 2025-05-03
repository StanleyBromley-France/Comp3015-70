#include "tileable_plane.h"
#include <vector>
#include <iostream>

TileablePlane::TileablePlane(float size, int segments, float tileFactor) {
    GenerateMeshData(size, segments, tileFactor);
}

void TileablePlane::GenerateMeshData(float size, int segments, float tileFactor) {
    std::vector<GLuint> indices;
    std::vector<GLfloat> points;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;

    // Generate vertex data
    for (int i = 0; i <= segments; ++i) {
        for (int j = 0; j <= segments; ++j) {
            // Position (XZ plane)
            float x = (float)j / segments * size - size / 2.0f;
            float z = (float)i / segments * size - size / 2.0f;
            points.insert(points.end(), { x, 0.0f, z });

            // Normal (straight up)
            normals.insert(normals.end(), { 0.0f, 1.0f, 0.0f });

            // Texture coordinates with tiling
            float u = (float)j / segments * tileFactor;
            float v = (float)i / segments * tileFactor;
            texCoords.insert(texCoords.end(), { u, v });
        }
    }

    // Generate indices
    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            GLuint topLeft = i * (segments + 1) + j;
            GLuint topRight = topLeft + 1;
            GLuint bottomLeft = (i + 1) * (segments + 1) + j;
            GLuint bottomRight = bottomLeft + 1;

            indices.insert(indices.end(), {
                topLeft, bottomLeft, topRight,
                topRight, bottomLeft, bottomRight
                });
        }
    }

    // Initialize buffers via base class
    initBuffers(&indices, &points, &normals, &texCoords);
}
