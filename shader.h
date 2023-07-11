#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
    enum ShaderType
    {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER,
        UNKNOWN
    };

private:
    // Полезные функции для проверки ошибок компиляции/связывания шейдеров
    void checkCompileErrors(GLuint shader, ShaderType type);
    void checkLinkingErrors(GLuint shader);
    // Функции чтения файлов
    void readVertexShader();
    void readFragmentShader();
    void readGeometryShader();
    // Функции компиляции
    void compileShaderProgram();
    unsigned int compileShader(ShaderType type);
    // Внутренняя конвертация по enum'у
    std::string getFilePath(ShaderType type) const;
    std::string getCode(ShaderType type)const;
    unsigned int getShaderId(ShaderType type);


public:
	
    // Конструктор генерирует шейдер "на лету"
    Shader(const std::string vertexPath, const std::string fragmentPath, const std::string geometryPath = "");
	
    // Активация шейдера
    void use() const;
	
    // Полезные uniform-функции
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;


    unsigned int ID() const;

private:
    std::string m_fileNameVertex;       // Имя файла с кодом вершинного шейдера
    std::string m_fileNameFragment;     // Имя файла с кодом фрагментного шейдера
    std::string m_fileNameGeometry;     // Имя файла с кодом геометрического шейдера

    std::string m_codeVertex;           // Код вершинного шейдера
    std::string m_codeFragment;         // Код фрагментного шейдера
    std::string m_codeGeometry;         // Код геометрического шейдера

    std::ifstream vShaderFile;          // Поток чтения файла с кодом вершинного шейдера
    std::ifstream fShaderFile;          //
    std::ifstream gShaderFile;          //

    const std::string SHADER_TYPE_STRING_VERTEX = "VERTEX";
    const std::string SHADER_TYPE_STRING_FRAGMENT = "FRAGMENT";
    const std::string SHADER_TYPE_STRING_GEOMETRY = "GEOMETRY";

    unsigned int m_idGeometry;
    unsigned int m_idVertex;
    unsigned int m_idFragment;
    unsigned int m_id = 0;

};
#endif
