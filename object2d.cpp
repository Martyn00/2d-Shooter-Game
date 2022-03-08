#include "object2d.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* objects2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(length, length, 0), color),
            VertexFormat(corner + glm::vec3(0, length, 0), color)};

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float l,
    float L,
    glm::vec3 color,
    bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(l, 0, 0), color),
            VertexFormat(corner + glm::vec3(l, L, 0), color),
            VertexFormat(corner + glm::vec3(0, L, 0), color)};

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2, 3};

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}


Mesh* objects2D::CreateTriangle(
    const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, color),
            VertexFormat(corner + glm::vec3(length, 0, 0), color),
            VertexFormat(corner + glm::vec3(0, length, 0), color)};

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2};

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        // indices.push_back(2);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}
