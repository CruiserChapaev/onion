#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // содержит все объявления OpenGL-типов

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h" // shader.h идентичен файлу shader_s.h
#include "Vertex.hpp"
#include "Texture.hpp"

#include <string>
#include <vector>



class Mesh {
public:
    // Конструктор
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

    // Рендеринг меша
    void Draw(const Shader& shader);

private:
    Mesh() = default;
    Mesh(const Mesh& anoter) = default;


    // Инициализируем все буферные объекты/массивы
    void setupMesh();

private:
    // Данные меша
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;
    // Данные для рендеринга
    unsigned int VBO, EBO;
};
#endif
