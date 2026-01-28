#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <vector>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 
#include <iostream>
#include <limits>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderable.h"

class Mesh : virtual public Renderable {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	//std::vector<glm::vec3> uvCoords;
	std::vector<int> indecies;

	GLuint VBO[3];
	GLuint EBO;

public:
	Mesh(aiMesh* mesh) {
		for (int i = 0; i < mesh -> mNumVertices; i++) {
			vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
			//normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
			//uvCoords.push_back(glm::vec3(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, 0.0));
		}

		for (int i = 0; i < mesh->mNumFaces; i++) {
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				indecies.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		normalize();

		glGenVertexArrays(1, &VAO); 
		glGenBuffers(3, VBO);
		glGenBuffers(1, &EBO);

		// Bind the VAO
		glBindVertexArray(VAO);

		// Bind VBOs and configure vertex attributes
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		/*glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);*/


		/*glBindBuffer(GL_ARRAY_BUFFER, this->VBO[2]);  // Bind UV VBO
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);  // Configure UV attribute
		glEnableVertexAttribArray(2);*/

		// Bind the EBO (Element Array Buffer)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(int), (void*)(&indecies[0]), GL_DYNAMIC_DRAW);

		// Unbind the VAO and VBOs
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	~Mesh() {
		/*glDeleteBuffers(3, VBO);
		glDeleteBuffers(1, (const GLuint*)(EBO));
		glDeleteVertexArrays(1, (const GLuint*)VAO);*/
	}
	void normalize() {
		std::pair<glm::vec3, glm::vec3> limits = getBoundingBox();
		glm::vec3 middle((limits.first.x + limits.second.x) / 2, (limits.first.y + limits.second.y) / 2, (limits.first.z + limits.second.z) / 2);
		glm::mat4 translate(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -middle.x, -middle.y, -middle.z, 1.0);
		float tmp = (std::max)(-limits.first.x + limits.second.x, -limits.first.y + limits.second.y);
		float M = (std::max)(tmp, -limits.first.z + limits.second.z);
		glm::mat4 scale(2.0f / M, 0.0, 0.0, 0.0, 0.0, 2.0f / M, 0.0, 0.0, 0.0, 0.0, 2.0f / M, 0.0, 0.0, 0.0, 0.0, 1.0);
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] = scale * translate * glm::vec4(vertices[i], 1.0);
		}
	}
	void draw(bool drawTriangles) const override {

		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements( (drawTriangles) ? GL_TRIANGLES : GL_LINES, indecies.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

private:
	std::pair<glm::vec3, glm::vec3> getBoundingBox() const {
		float xmax = std::numeric_limits<float>::lowest();
		float xmin = (std::numeric_limits<float>::max)();
		float ymax = std::numeric_limits<float>::lowest();
		float ymin = (std::numeric_limits<float>::max)();
		float zmax = std::numeric_limits<float>::lowest();
		float zmin = (std::numeric_limits<float>::max)();

		for (int i = 0; i < vertices.size(); i++) {
			if (vertices[i].x > xmax) xmax = vertices[i].x;
			if (vertices[i].x < xmin) xmin = vertices[i].x;
			

			if (vertices[i].y > ymax) ymax = vertices[i].y;
			if (vertices[i].y < ymin) ymin = vertices[i].y;
			

			if (vertices[i].z > zmax) zmax = vertices[i].z;
			if (vertices[i].z < zmin) zmin = vertices[i].z;
		}
		return { {xmin, ymin, zmin}, {xmax, ymax, zmax}};
	}
};