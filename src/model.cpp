#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include "stb_image.h" //TODO DELETE WHEN NO LONGER USING Texture

using std::cout, std::endl;
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

model::model(string const &path, bool flipped_textures) {
        load_model(path, flipped_textures);
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void model::load_model(string const &path, const bool flipped_textures) {
        //Assimp::DefaultLogger::create(string(path + "log.txt").c_str(), Assimp::Logger::VERBOSE);
        Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
        Assimp::LogStream* stderrStream = Assimp::LogStream::createDefaultStream(aiDefaultLogStream_STDERR);
        Assimp::DefaultLogger::get()->attachStream(stderrStream, Assimp::Logger::NORMAL | Assimp::Logger::Debugging | Assimp::Logger::VERBOSE);

        // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {// if is Not Zero
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    process_node(scene->mRootNode, scene, flipped_textures);
        Assimp::DefaultLogger::kill();
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void model::process_node(aiNode *node, const aiScene *scene, const bool flipped_textures) {
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene, flipped_textures));
    }
    // after we've processed all meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene, flipped_textures);
    }
}

mesh_item model::process_mesh(aiMesh *mesh, const aiScene *scene, const bool flipped_textures) {
    // data to fill
    vector<vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) {// does the mesh contain texture coordinates?
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<Texture> diffuseMaps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse", flipped_textures);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular", flipped_textures);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal", flipped_textures);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height", flipped_textures);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return mesh_item(vertices, indices, textures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
vector<Texture> model::load_material_textures(aiMaterial *mat, aiTextureType type, string typeName, const bool /*flipped_textures*/)
{
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip) {   // if texture hasn't been loaded already, load it
            Texture texture;
//            std::string location = "../assets/backpack/" + std::string(str.C_Str());
//            texture.wrap_parameters();
//            texture.load_image(location, flipped_textures);
//            texture.type = typeName;
//            texture.path = str.C_Str();
//            textures.push_back(texture);
//            textures_loaded.emplace_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessarily load duplicate textures.
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessarily load duplicate textures.
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char *path, const string &directory, bool /*gamma*/)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = 0;
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
        std::cout << "Loaded texture from path: " << path << std::endl;
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
