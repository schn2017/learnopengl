#include <assimp/Importer.hpp>
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <string>
#include <vector>

class Mesh;
class Shader;
struct Texture;

class Model
{
    public:
        Model(char* path);

        void draw(Shader& shader);
    private:
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

        std::vector<Mesh> m_meshes;
        std::string m_directory;
};