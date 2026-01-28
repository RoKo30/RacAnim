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
	std::vector<glm::vec3> colors;
	std::vector<glm::vec2> texCoords;
	std::vector<int> indices;

	GLuint VBO[4]; // 0 - vertices, 1 - normals, 2 - colors, 4 - uvs
	GLuint EBO;
	GLuint VAO;

public:
	Mesh(aiMesh* mesh) {
		// Load vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			vertices.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

			// Load normals if available
			if (mesh->HasNormals()) {
				normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
			if (mesh->HasTextureCoords(0)) {
				texCoords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}
		}

		// Load face indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}
		computeVertexNormalsIfMissing();
		normalize();

		glGenVertexArrays(1, &VAO); 
		glGenBuffers(4, VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		// Position VBO [location = 0]
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		// Normal VBO [location = 1]
		if (!normals.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);
		}

		// Color buffer will be filled optionally later if needed [location = 2]
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(3);
		// EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Mesh(const std::vector<glm::vec3>& verts,
		const std::vector<glm::vec2>& uvs,
		const std::vector<int>& inds)
		: vertices(verts), texCoords(uvs), indices(inds)
	{
		normals.resize(vertices.size(), glm::vec3(0, 0, 1));

		glGenVertexArrays(1, &VAO);
		glGenBuffers(4, VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		// Positions
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(glm::vec3),
			vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		// Normals
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER,
			normals.size() * sizeof(glm::vec3),
			normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);

		// UVs
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER,
			texCoords.size() * sizeof(glm::vec2),
			texCoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(3);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(int),
			indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void draw(bool drawTriangles) const override {
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, drawTriangles ? GL_FILL : GL_LINE);
		glDrawElements(drawTriangles ? GL_TRIANGLES : GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}


	void generateRandomColorsPerTriangle() {
		std::vector<glm::vec3> newVertices;
		std::vector<glm::vec3> newNormals;
		std::vector<glm::vec3> newColors;
		std::vector<int> newIndices;

		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		for (size_t i = 0; i < indices.size(); i += 3) {
			glm::vec3 color(
				static_cast<float>(rand()) / RAND_MAX,
				static_cast<float>(rand()) / RAND_MAX,
				static_cast<float>(rand()) / RAND_MAX
			);

			for (int j = 0; j < 3; ++j) {
				int idx = indices[i + j];
				newVertices.push_back(vertices[idx]);
				if (!normals.empty()) newNormals.push_back(normals[idx]);
				newColors.push_back(color);
				newIndices.push_back(static_cast<int>(newVertices.size() - 1));
			}
		}

		vertices = newVertices;
		normals = newNormals;
		colors = newColors;
		indices = newIndices;

		glBindVertexArray(VAO);

		// Reupload position
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		// Reupload normals 
		if (!normals.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);
		}

		// Upload colors to VBO[2] [location = 2]
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(2);

		// Reupload EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

private:
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

	void computeVertexNormalsIfMissing() {
		if (!normals.empty()) return; // Already have normals

		normals.resize(vertices.size(), glm::vec3(0.0f));

		for (size_t i = 0; i < indices.size(); i += 3) {
			int i0 = indices[i];
			int i1 = indices[i + 1];
			int i2 = indices[i + 2];

			glm::vec3 v0 = vertices[i0];
			glm::vec3 v1 = vertices[i1];
			glm::vec3 v2 = vertices[i2];

			glm::vec3 edge1 = v1 - v0;
			glm::vec3 edge2 = v2 - v0;
			glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

			// Add face normal to each vertex normal
			normals[i0] += faceNormal;
			normals[i1] += faceNormal;
			normals[i2] += faceNormal;
		}

		// Normalize all normals
		for (glm::vec3& n : normals) {
			n = glm::normalize(n);
		}
	}

};

std::shared_ptr<Mesh> createQuadMesh() {
	std::vector<glm::vec3> vertices = {
		{-0.5f, -0.5f, 0.0f},
		{ 0.5f, -0.5f, 0.0f},
		{ 0.5f,  0.5f, 0.0f},
		{-0.5f,  0.5f, 0.0f}
	};

	std::vector<glm::vec2> uvs = {
		{0, 0}, {1, 0}, {1, 1}, {0, 1}
	};

	std::vector<int> indices = {
		0, 1, 2,
		2, 3, 0
	};

	return std::make_shared<Mesh>(vertices, uvs, indices);
}