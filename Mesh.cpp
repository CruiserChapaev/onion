#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // Теперь, когда у нас есть все необходимые данные, устанавливаем вершинные буферы и указатели атрибутов
    setupMesh();
}

void Mesh::Draw(const Shader& shader)
{
    // Связываем соответствующие текстуры
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // перед связыванием активируем нужный текстурный юнит

        // Получаем номер текстуры (номер N в diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // конвертируем unsigned int в строку
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // конвертируем unsigned int в строку
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // конвертируем unsigned int в строку

        // Теперь устанавливаем сэмплер на нужный текстурный юнит
        glUniform1i(glGetUniformLocation(shader.ID(), (name + number).c_str()), static_cast<GLint>(i));
        // и связываем текстуру
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Отрисовываем меш
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // Считается хорошей практикой возвращать значения переменных к их первоначальным значениям
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    // Создаем буферные объекты/массивы
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Загружаем данные в вершинный буфер
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Самое замечательное в структурах то, что расположение в памяти их внутренних переменных является последовательным.
    // Смысл данного трюка в том, что мы можем просто передать указатель на структуру, и она прекрасно преобразуется в массив данных с элементами типа glm::vec3 (или glm::vec2), который затем будет преобразован в массив данных float, ну а в конце – в байтовый массив
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Устанавливаем указатели вершинных атрибутов

    // Координаты вершин
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    // Нормали вершин
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    // Текстурные координаты вершин
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));

    // Касательный вектор вершины
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));

    // Вектор бинормали вершины
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

    glBindVertexArray(0);
}
