#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model 
{
public:
    /**
     * @brief Model - Конструктор в качестве аргумента использует путь к 3D-модели.
     * @param path - Передаваемый путь к модели.
     * @param gamma - значение наличия гамма коррекции (по умолчанию false).
     */
    Model(string const &path, bool gamma = false);

    ~Model();

    /**
     * @brief Draw - Отрисовываем модель, а значит и все её меши.
     * @param shader - Объект шейдера для использования.
     */
    void Draw(Shader& shader);
    
private:
    /**
     * @brief loadModel - Загружаем модель с помощью Assimp и сохраняем полученные меши в векторе meshes.
     * @param path - Путь до модели.
     */
    void loadModel(string const &path);

    /**
     * @brief processNode -  Рекурсивная обработка узла. Обрабатываем каждый отдельный меш,
     * расположенный в узле, и повторяем этот процесс для своих дочерних углов (если таковы вообще имеются).
     * @param node - Текущий узел.
     * @param scene - Текущая сцена.
     */
    void processNode(aiNode *node, const aiScene *scene);

    /**
     * @brief processMesh
     * @param mesh
     * @param scene
     * @return
     */
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene);

    /**
     * @brief loadMaterialTextures - Проверяем все текстуры материалов заданного типа и загружам текстуры,
     * если они еще не были загружены. Необходимая информация возвращается в виде структуры Texture.
     * @param mat
     * @param type
     * @param typeName
     * @return
     */
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

private:
    // Данные модели
    vector<Texture>     m_texturesLoaded;   // (оптимизация) сохраняем все загруженные текстуры, чтобы убедиться, что они не загружены более одного раза
    vector<Mesh*>       m_meshes;
    string              m_directory;
    bool                m_gammaCorrection;

};

#endif

