#include "mesh_item.h"
#include <glm/gtc/matrix_transform.hpp>

mesh_item::mesh_item(vector<vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setup();
}

void mesh_item::draw(Shader &shader) {
    // bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if(name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if(name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        shader.setInt(name + number, i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id); //NIKOS TODO
        //textures[i].bind();
    }

    // draw mesh
    vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    vao.unbind();

    // always good practice: set everything back to default once configured.
    glActiveTexture(GL_TEXTURE0);
}

void mesh_item::setup() {
    // create buffers/arrays
    // load data into vertex buffers
    vao.bind();

    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct, and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
    vbo.add_buffer((float*)&vertices[0],vertices.size() * sizeof(vertex));
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    ebo.add_buffer((unsigned int*)&indices[0], indices.size() * sizeof(unsigned int));

    // set the vertex attribute pointers
    // vertex Positions
    vao.add_attribute(0, 3, sizeof(vertex), 0);

    // vertex normals
    vao.add_attribute(1, 3, sizeof(vertex), offsetof(vertex, Normal));

    // vertex texture coords
    vao.add_attribute(2, 2, sizeof(vertex), offsetof(vertex, TexCoords));

    // vertex tangent
    vao.add_attribute(3, 3, sizeof(vertex), offsetof(vertex, Tangent));

    // vertex bitangent
    vao.add_attribute(4, 3, sizeof(vertex), offsetof(vertex, Bitangent));

    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(vertex), (void*)offsetof(vertex, m_BoneIDs));
    //NIKOS TODO: vao.add_attribute(5, 4, sizeof(vertex), offsetof(vertex, m_BoneIDs));

    // weights
    vao.add_attribute(6, 4, sizeof(vertex), offsetof(vertex, m_Weights));

    vao.unbind();
}
