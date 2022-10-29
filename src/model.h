#ifndef MODEL_H
#define MODEL_H
#include "shader.h"
#include "mesh_item.h"
//#include "texture.h"
#include <assimp/scene.h>
#include <string>
#include <vector>
using std::string, std::vector;

class model
{
public:
    // constructor, expects a filepath to a 3D model.
    //explicit model(string const &path, bool flipped_textures = true, bool gamma = false) : gammaCorrection(gamma) {
    explicit model(string const &path, bool flipped_textures = true);

    // draw model ie all its meshes
    void draw(Shader &shader) {
        for (auto &mesh: meshes)
            mesh.draw(shader);
    }
    void draw_instanced(GLuint amount) {
        for (auto& mesh: meshes)
            mesh.draw_instanced(amount);
    }
    void set_instanced_glm4data_every_mesh(GLuint position) {
        for (auto &mesh : meshes)
            mesh.set_instanced_glm4data(position);
    }

    // model data
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
private:
    vector<mesh_item> meshes;
    string directory;
    //bool gammaCorrection;

    void load_model(string const& path, bool flipped_textures);
    void process_node(aiNode *node, const aiScene *scene, bool flipped_textures);
    mesh_item process_mesh(aiMesh *mesh, const aiScene *scene, bool flipped_textures);
    vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string typeName, bool flipped_textures);
};
#endif
