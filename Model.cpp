#include "Model.h"
#include "ImageLoader.h"

Model::Model(char* path, ShaderProgram p, bool customShader){
  this->model = glm::mat4(1.0f);
  if(customShader){
    this->prog = p;
  }
  this->customShader = customShader;
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);
  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
    std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
  }
  {
    std::string p(path);
    this->directory = p.substr(0, p.find_last_of('/'));
  }
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene){
  for(unsigned int i = 0; i < node->mNumMeshes; i++){
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(this->loadMesh(mesh, scene));
  }
  for(unsigned int i = 0; i < node->mNumChildren; i++){
    processNode(node->mChildren[i], scene);
  }
}

GLuint loadTexture(const char* path, std::string dir){
  std::string loadPath = dir + '/' + std::string(path);

  GLuint id;
  glGenTextures(1, &id);
  int w, h, c;
  unsigned char* img = loadImg(loadPath.c_str(), &w, &h, &c);
  glBindTexture(GL_TEXTURE_2D, id);
  switch(c){
    case 1:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, img);
    break;
    case 3:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    break;
    case 4:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    break;
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  freeImg(img);
  glBindTexture(GL_TEXTURE_2D, 0);

  return id;
}

Mesh Model::loadMesh(aiMesh *mesh, const aiScene *scene){
  std::vector<Vertex> vertices;
  std::vector<Texture> textures;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures_loaded;

  for(unsigned int i = 0; i < mesh->mNumVertices; i++){
    Vertex v;
    glm::vec3 p, n;
    glm::vec2 tc;

    p.x = mesh->mVertices[i].x;
    p.y = mesh->mVertices[i].y;
    p.z = mesh->mVertices[i].z;
    v.position = p;
    n.x = mesh->mNormals[i].x;
    n.y = mesh->mNormals[i].y;
    n.z = mesh->mNormals[i].z;
    v.normal = n;
    if(mesh->mTextureCoords[0]){
      tc.x = mesh->mTextureCoords[0][i].x;
      tc.y = mesh->mTextureCoords[0][i].y;
      v.texCoords = tc;
    }else v.texCoords = glm::vec2(0.0f, 0.0f);

    vertices.push_back(v);
  }

  for(unsigned int i = 0; i < mesh->mNumFaces; i++){
    aiFace face = mesh->mFaces[i];
    for(unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
  }

  if(mesh->mMaterialIndex >= 0){
    aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
    for(unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++){
      aiString s;
      bool isLoaded = false;
      mat->GetTexture(aiTextureType_DIFFUSE, i, &s);
      for(int j = 0; j < textures_loaded.size(); j++){
        if(std::strcmp(textures_loaded[j].path.c_str(), s.C_Str())==0){
          isLoaded = true;
          textures.push_back(textures_loaded[j]);
        }
      }

      if(!isLoaded){
        Texture t;
        t.texHnd = loadTexture(s.C_Str(), this->directory);
        t.isDiffuse = true;
        t.path = std::string(s.C_Str());
        textures.push_back(t);
      }
    }
  }
  return Mesh(vertices, textures, indices);
}



void Model::draw(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool updateModel){
  if(updateModel){
    this->model = glm::mat4(1.0f);
    this->model = glm::scale(glm::rotate(glm::translate(this->model, pos), glm::radians(180.0f), rot), scale);
  }

  if(this->customShader){
    for(unsigned int i = 0; i < this->meshes.size();i++){
      (this->meshes)[i].draw(this->model, &(this->prog));
    }
  }else{
    for(unsigned int i = 0; i < this->meshes.size();i++){
      (this->meshes)[i].draw(this->model, NULL);
    }
  }
}

void Model::enableCustomShader(){
  this->customShader = true;
}

void Model::disableCustomShader(){
  this->customShader = false;
}

void Model::toggleCustomShader(){
  this->customShader = !(this->customShader);
}
