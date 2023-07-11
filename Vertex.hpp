#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;     // Позиция
    glm::vec3 normal;       // Нормаль
    glm::vec2 texCoords;    // Текстурные координаты
    glm::vec3 tangent;      // Касательный вектор
    glm::vec3 bitangent;    // Вектор бинормали (вектор, перпендикулярный касательному вектору и вектору нормали)

};

#endif // VERTEX_HPP
