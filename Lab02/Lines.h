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

class Lines : virtual public Renderable {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;
	GLuint VBO;

public:
	Lines(std::vector<glm::vec3> vertices) {
		this->vertices = vertices;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		// Bind the VAO
		glBindVertexArray(VAO);

		// Bind VBOs and configure vertex attributes
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Unbind the VAO and VBOs
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void draw(bool drawTriangles) const override {
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

		glBindVertexArray(0);
	};

	std::vector<glm::vec3> getVertices() {
		return vertices;
	}

};