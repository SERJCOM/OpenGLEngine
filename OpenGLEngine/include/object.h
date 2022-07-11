#pragma once
#include <iostream>
#include "model.h"
#include "mesh.h"
#include "light.h"
#include "shader.h"
#include "texture.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

/// <summary>
/// ���������� ����������� ������� Object
/// 1)	��������� ������ 
/// 2)	������� ������, ��������� � �������������, ����������
/// 3)	
/// </summary>



struct Tree {
    std::vector <Tree> children;
    std::vector <int> meshIndex;
    std::string name;
    std::map <std::string, int>* meshNames;


    void createChild(std::string name) {
        Tree child;
        child.name = name;
        children.push_back(child);
    }

    void addMesh(int i) {
        meshIndex.push_back(i);
    }

    Tree* findNodeByName(std::string name) {
        for (int i = 0; i < children.size(); i++) {
            if (children[i].name == name) {
                return &children[i];
            }
        }
        return nullptr;
    }
    
    void GetIndexFromChildren(std::vector <int>* array, Tree* node) {

        array->push_back(meshNames->operator[](node->name)); // ������� ������ �� ����� node->name � �������� ��� �������� � ������

        for (int i = 0; i < node->children.size(); i++) {
            GetIndexFromChildren(array, &node->children[i]);
            std::cout << node->children[i].name << std::endl;
        }

    }

    std::vector <int>* GetIndexFromNode(std::string name, std::map <std::string, int>* meshNames) {
        this->meshNames = meshNames;
        Tree* firstNode = findNodeByName(name);

        if (firstNode == nullptr) { return nullptr; }
        else {
            std::vector <int>* array;
            GetIndexFromChildren(array, firstNode);
            return array;
        }
    }

};

class Object {
public:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<texture> textures_loaded;
    std::map <std::string, int> meshNames;
    bool gammaCorrection;

    Object(std::string path, PhysicsWorld* physworld)
    {
        this->physworld = physworld;
        loadModel(path);
    }

    void Draw(Shader& shader)
    {

        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    void ScaleMesh(std::string name, glm::vec3 size) {
        if (meshNames.find(name) == meshNames.end())    std::cout << "the element was not found\n";
        else    meshes[meshNames[name]].meshScale = size;
    }

    void ScaleObject(glm::vec3 size) {
        for (int i = 0; i < meshes.size(); i++) {
            meshes[i].meshScale = size;
        }
    }


    void RotateMesh(std::string name, float anglex, float angley, float anglez) {
        meshes[meshNames[name]].RotateMesh(anglex, angley, anglez);
    }

    void RotateObject(float anglex, float angley, float anglez) {
        for (int i = 0; i < meshes.size(); i++) {
            meshes[i].RotateMesh(anglex, angley, anglez);
        }
    }

    void SetMeshRotation(std::string name, float anglex, float angley, float anglez) {
        meshes[meshNames[name]].SetRotateMesh(anglex, angley, anglez);
    }

    void SetObjectRotation(float anglex, float angley, float anglez) {
        for (int i = 0; i < meshes.size(); i++) {
            meshes[i].RotateMesh(anglex, angley, anglez);
        }
    }



    void MoveObject(float x, float y, float z) {
        for (int i = 0; i < meshes.size(); i++) {
            meshes[i].MoveObject(glm::vec3(x, y, z));
        }
    }

    void MoveMesh(std::string name, float x, float y, float z) {
        if (meshNames.find(name) == meshNames.end())    std::cout << "the element was not found\n";
        else    meshes[meshNames[name]].MoveObject(glm::vec3(x, y, z));
    }

    void SetMeshPosition(std::string name, float x, float y, float z) {
        if (meshNames.find(name) == meshNames.end())    std::cout << "the element was not found\n";
        else    meshes[meshNames[name]].SetObjectPosition(glm::vec3(x, y, z));
    }

    void SetObjectPosition(float x, float y, float z) {
        for (int i = 0; i < meshes.size(); i++) 
            meshes[i].SetObjectPosition(glm::vec3(x, y, z));
    }



    void EnablePhysics(bool enable) { this->PhysicBool = enable; }

    static float DegToRad(float angle) {
        return angle * 3.13 / 180;
    }

private:
    bool PhysicBool = true;
    PhysicsWorld* physworld;
    int number = 0;
    std::string rootName;
    //Tree nodeTree;
   

    void loadModel(std::string path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        rootName = scene->mRootNode->mName.C_Str();
        std::cout << "the root name is " << rootName << std::endl;
        processNode(scene->mRootNode, scene, number);
    }

    void processNode(aiNode* node, const aiScene* scene, int index)
    {
        int ind = index;
        ind++;

        //noda->name = node->mName.C_Str();
        
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
            std::cout << mesh->mName.C_Str() << std::endl;
            meshNames[node->mName.C_Str()] = meshes.size()-1;
            //noda->addMesh(meshes.size() - 1);
            if (PhysicBool == true) {
                meshes[meshes.size() - 1].SetupPhysic(physworld);
                meshes[meshes.size() - 1].CreateRigidBody();
            }
        }


        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            Tree child;
            child.name = node->mChildren[i]->mName.C_Str();
            //noda->children.push_back(child);
    
            processNode(node->mChildren[i], scene, ind);
        }
        /*for (int i = 0; i < ind; i++) {
            cout << "=";
        }
        cout << node->mName.C_Str() << endl;*/
    }


    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<texture> textures;


        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;


                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }



        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }





        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


        std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        
        return Mesh(vertices, indices, textures);
    }

    std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                texture texturee;
                texturee.id = LoadTextureFromFile(str.C_Str(), this->directory);
                texturee.type = typeName;
                texturee.path = str.C_Str();
                textures.push_back(texturee);
                textures_loaded.push_back(texturee);
            }
        }
        return textures;
    }

};

