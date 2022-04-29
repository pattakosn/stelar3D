#ifndef MODEL_H
#define MODEL_H
#include "shader.h"
#include "mesh_item.h"
//#include "texture.h"
#include <assimp/scene.h>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include <string>
#include <vector>
using std::string, std::vector;

class model
{
public:
    // constructor, expects a filepath to a 3D model.
    explicit model(string const &path, bool flipped_textures = true, bool gamma = false) : gammaCorrection(gamma) {
        //Assimp::DefaultLogger::create(string(path + "log.txt").c_str(), Assimp::Logger::VERBOSE);
        Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
        Assimp::LogStream* stderrStream = Assimp::LogStream::createDefaultStream(aiDefaultLogStream_STDERR);
        Assimp::DefaultLogger::get()->attachStream(stderrStream, Assimp::Logger::NORMAL | Assimp::Logger::Debugging | Assimp::Logger::VERBOSE);

        load_model(path, flipped_textures);

        Assimp::DefaultLogger::kill();
    }

    // draw model ie all its meshes
    void draw(Shader &shader) {
        for (auto &mesh: meshes)
            mesh.draw(shader);
    }
private:
    // model data
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<mesh_item> meshes;
    string directory;
    bool gammaCorrection;

    void load_model(string const& path, bool flipped_textures);
    void process_node(aiNode *node, const aiScene *scene, bool flipped_textures);
    mesh_item process_mesh(aiMesh *mesh, const aiScene *scene, bool flipped_textures);
    vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string typeName, bool flipped_textures);
};
#endif
