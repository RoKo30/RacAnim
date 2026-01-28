#pragma once
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 

#include "stb_image.h"


// Standard Headers
#include <iostream>
#include <cstdlib>
class MeshLoader {

public:
	static const aiScene* loadMesh(Assimp::Importer& importer, std::string path, std::string fileName) {
		std::string dirPath(path, 0, path.find_last_of("\\/"));
		std::string resPath(dirPath);
		resPath.append("\\resources");
		std::string objPath(resPath);
		//objPath.append("\\glava\\glava.obj"); 
		objPath.append("\\").append(fileName);
		std::cout << objPath << "\n";
		const aiScene* scene = importer.ReadFile(objPath.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs |
			aiProcess_GenNormals |
			aiProcess_PreTransformVertices | //
			aiProcess_ConvertToLeftHanded //
		);

		if (!scene) {
			std::cerr << importer.GetErrorString();
			return nullptr;
		}

		return scene;
	}

	static const aiScene* loadMesh(Assimp::Importer& importer, std::string path, std::string fileName, bool poinCloud) {
		std::string dirPath(path, 0, path.find_last_of("\\/"));
		std::string resPath(dirPath);
		resPath.append("\\resources");
		std::string objPath(resPath);
		//objPath.append("\\glava\\glava.obj"); 
		objPath.append("\\").append(fileName);
		std::cout << objPath << "\n";
		const aiScene* scene = importer.ReadFile(objPath.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs |
			aiProcess_GenNormals |
			aiProcess_PreTransformVertices | //
			aiProcess_ConvertToLeftHanded //
		);

		if (!scene) {
			std::cerr << importer.GetErrorString();
			return nullptr;
		}

		return scene;
	}

};