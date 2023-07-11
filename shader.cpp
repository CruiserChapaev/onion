#include "shader.h"

Shader::Shader(const std::string vertexPath,
               const std::string fragmentPath,
               const std::string geometryPath)
    : m_fileNameVertex(vertexPath),
      m_fileNameFragment(fragmentPath),
      m_fileNameGeometry(geometryPath)
{
    try
    {
        readVertexShader();
        readFragmentShader();
        if (m_codeGeometry.size() != 0)
        {
            readGeometryShader();
        }
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::CAN'T READ FILE " << std::endl;
    }
    m_idVertex = compileShader(VERTEX);
    m_idFragment = compileShader(FRAGMENT);
    if (m_fileNameGeometry.size())
    {
        m_idGeometry = compileShader(GEOMETRY);
    }
    compileShaderProgram();
}



void Shader::use() const
{
    glUseProgram(m_id);
}



void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}



void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}



void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}



void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}



void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
}



void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}



void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
}



void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}



void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
}



void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



void Shader::checkCompileErrors(GLuint shader, ShaderType type)
{
    GLint success;
    GLchar infoLog[1024];
    std::string unitsFileName;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR file: " << getFilePath(type)
                  << "\nDetails :\n" << infoLog << std::endl;
    }
}



void Shader::checkLinkingErrors(GLuint shader)
{
    GLint success;
    GLchar infoLog[1024];
    std::string unitsFileName;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR file: "
                  << "\nDetails :\n" << infoLog << std::endl;
    }
}



void Shader::readVertexShader()
{
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vShaderFile.open(m_fileNameVertex);
    std::stringstream vShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    vShaderFile.close();
    m_codeVertex = vShaderStream.str();
}



void Shader::readFragmentShader()
{
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.open(m_fileNameFragment);
    std::stringstream fShaderStream;
    fShaderStream << fShaderFile.rdbuf();
    fShaderFile.close();
    m_codeFragment = fShaderStream.str();
}



void Shader::readGeometryShader()
{
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.open(m_fileNameGeometry);
    std::stringstream gShaderStream;
    gShaderStream << gShaderFile.rdbuf();
    gShaderFile.close();
    m_codeGeometry = gShaderStream.str();
}



unsigned int Shader::compileShader(Shader::ShaderType type)
{
    std::string codeCopy = getCode(type);
    const char* code = codeCopy.c_str();
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, type);
    return shader;
}



void Shader::compileShaderProgram()
{
    m_id = glCreateProgram();
    glAttachShader(m_id, m_idVertex);
    glAttachShader(m_id, m_idFragment);
    if (!m_codeGeometry.empty())
        glAttachShader(m_id, m_idGeometry);
    glLinkProgram(m_id);
    checkLinkingErrors(m_id);
    // После того, как мы связали шейдеры с нашей программой, удаляем их, т.к. они нам больше не нужны
    glDeleteShader(m_idVertex);
    glDeleteShader(m_idFragment);
    if (!m_codeGeometry.empty())
        glDeleteShader(m_idGeometry);
}



std::string Shader::getFilePath(Shader::ShaderType type) const
{
    switch (type) {
        case VERTEX:
            return m_fileNameVertex;
        case FRAGMENT:
            return m_fileNameFragment;
        case GEOMETRY:
            return m_fileNameGeometry;
        default:
            return "FILE NOT DEFIED";
    }
}



std::string Shader::getCode(Shader::ShaderType type) const
{
    switch (type) {
        case VERTEX:
            return m_codeVertex;
        case FRAGMENT:
            return m_codeFragment;
        case GEOMETRY:
            return m_codeGeometry;
        default:
            return "FILE NOT DEFIED";
    }
}



unsigned int Shader::getShaderId(Shader::ShaderType type)
{
    switch (type) {
        case VERTEX:
            return m_idVertex;
        case FRAGMENT:
            return m_idFragment;
        case GEOMETRY:
            return m_idGeometry;
        default:
            return 0;
    }
}



unsigned int Shader::ID() const
{
    return m_id;
}
