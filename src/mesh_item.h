#ifndef MESH_ITEM_H
#define MESH_ITEM_H
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"
#include "element_array.h"
//#include "texture.h"

#define MAX_BONE_INFLUENCE 4

using std::vector, std::string;

struct vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    // bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class mesh_item {
public:
    mesh_item(vector<vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

    // render mesh
    void draw(Shader &shader);

    void set_instanced_glm4data(GLuint position) {
        vao.bind();
        for (int i = 0; i < 4; ++i) {
            vao.add_attribute(position + i, 4, sizeof(glm::mat4), i * sizeof(glm::vec4));
            glVertexAttribDivisor(position + i, 1);
        }
        attributes_binding_object::unbind();
    }
    void draw_instanced(GLuint amount) {
        vao.bind();
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0, amount);
        vao.unbind();
    }
    // mesh Data
    vector<vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
private:
    // render data
    attributes_binding_object vao;
    vertex_array vbo;
    element_array ebo;

    // initialize all buffer objects/arrays
    void setup();
};
#endif
