#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <chrono>
#include "Transform.h"
#include "Lines.h"

class Camera : public Transform {
	float zoom;
	std::vector<glm::vec3> path;
	size_t pathIndex = 0;
	bool moving = false;
	float moveDelay = 0.05f; // sec
	float lastMoveTime = 0.0f;

public:
	Camera(float zoom) : Transform(0), zoom(zoom) {}
	Camera() : Transform(0){}

	glm::mat4 getPerspectiveMatrix(glm::vec2 screenSize) {
		return glm::perspective(glm::radians(45.0f), screenSize.x / screenSize.y, 1.0f, 100.0f);
	}

	/*glm::mat4 getPerspectiveMatrix(glm::vec2 screenSize) {
		return MyGLM::frustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 100.0f);
	}*/

	void startMoveOnCurve(const std::vector<glm::vec3>& vertices) {
		path = vertices;
		pathIndex = 0;
		moving = true;
		lastMoveTime = glfwGetTime();
	}

	void updateMovement() {
		if (!moving || pathIndex >= path.size()) return;

		float currentTime = glfwGetTime();
		if (currentTime - lastMoveTime >= moveDelay) {
			setPosition(path[pathIndex]);
			pathIndex++;
			lastMoveTime = currentTime;

			if (pathIndex >= path.size()) {
				moving = false;
			}
		}
	}
};