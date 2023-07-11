#include "model.h"

Model::Model(const string& path, bool gamma) : m_gammaCorrection(gamma)
{
    loadModel(path);
}



Model::~Model()
{
    for (Mesh* ptr : m_meshes)
    {
        delete ptr;
    }
    m_meshes.clear();
}



void Model::Draw(Shader& shader)
{
    for(unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes[i]->Draw(shader);
}



void Model::loadModel(const string& path)
{
    // Чтение файла с помощью Assimp
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    // Проверка на ошибки
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // если НЕ 0
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }

    // Получение пути к файлу
    m_directory = path.substr(0, path.find_last_of('/'));

    // Рекурсивная обработка корневого узла Assimp
    processNode(scene->mRootNode, scene);
}



void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Обрабатываем каждый меш текущего узла
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // Узел содержит только индексы объектов в сцене.
        // Сцена же содержит все данные; узел - это лишь способ организации данных
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // После того, как мы обработали все меши (если таковые имелись), мы начинаем рекурсивно обрабатывать каждый из дочерних узлов
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}



Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // Данные для заполнения
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // Цикл по всем вершинам меша
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // мы объявляем промежуточный вектор, т.к. Assimp использует свой собственный векторный класс,
        // который не преобразуется напрямую в тип glm::vec3,
        // поэтому сначала мы передаем данные в этот промежуточный вектор типа glm::vec3
        glm::vec3 vector;

        // Координаты
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // Нормали
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        // Текстурные координаты
        if(mesh->mTextureCoords[0]) // если меш содержит текстурные координаты
        {
            glm::vec2 vec;

            // Вершина может содержать до 8 различных текстурных координат. Мы предполагаем, что мы не будем использовать модели,
            // в которых вершина может содержать несколько текстурных координат, поэтому мы всегда берем первый набор (0)
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        // Касательный вектор
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.tangent = vector;

        // Вектор бинормали
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.bitangent = vector;
        vertices.push_back(vertex);
    }
    // Теперь проходимся по каждой грани меша (грань - это треугольник меша) и извлекаем соответствующие индексы вершин
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        // Получаем все индексы граней и сохраняем их в векторе indices
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Обрабатываем материалы
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // Мы вводим соглашение об именах сэмплеров в шейдерах. Каждая диффузная текстура будет называться 'texture_diffuseN',
    // где N - порядковый номер от 1 до MAX_SAMPLER_NUMBER.
    // Тоже самое относится и к другим текстурам:
    // диффузная - texture_diffuseN
    // отражения - texture_specularN
    // нормали - texture_normalN

    // 1. Диффузные карты
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, /*"2k_sun"*/ "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. Карты отражения
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, /*"2k_sun"*/ "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. Карты нормалей
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. Карты высот
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // Возвращаем меш-объект, созданный на основе полученных данных
    return new Mesh(vertices, indices, textures);
}



vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Проверяем, не была ли текстура загружена ранее, и если - да, то пропускаем загрузку новой текстуры и переходим к следующей итерации
        bool skip = false;
        for(unsigned int j = 0; j < m_texturesLoaded.size(); j++)
        {
            if(std::strcmp(m_texturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_texturesLoaded[j]);
                skip = true; // текстура с тем же путем к файлу уже загружена, переходим к следующей (оптимизация)
                break;
            }
        }
        if(!skip)
        {   // если текстура еще не была загружена, то загружаем её
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->m_directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            // сохраняем текстуру в массиве с уже загруженными текстурами,
            // тем самым гарантируя, что у нас не появятся без необходимости дубликаты текстур.
            m_texturesLoaded.push_back(texture);
        }
    }
    return textures;
}



unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    static_cast<void>(gamma);
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
