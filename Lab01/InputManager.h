#pragma once

#include <glad/glad.h>
#include <string>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "Camera.h"
#include "Lines.h"
#include "BezieBuilder.h"
#include "BSpline.h"


// Standard Headers
#include <iostream>

namespace im{
	static Camera* camera;
	static Renderer* renderer;
	static Shader* shader;
	static Object* controlledObject = nullptr;
	static Bspline* spline = nullptr;

	std::vector<glm::vec3> lineVerts;

	std::shared_ptr<Lines> prevPolyLine;
	std::shared_ptr<Lines> prevInterpLine;
	std::shared_ptr<Lines> prevBezierLine;

	static double lastX = 0.0, lastY = 0.0;
	static bool firstMouse = true;
	float sensitivity = 0.1;
	bool showMouse = false;

	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
			camera->localMove(glm::vec3(0.0, 0.0, 0.1f));
		}

		if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
			camera->localMove(glm::vec3(-0.1f, 0.0, 0.0));
		}

		if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
			camera->localMove(glm::vec3(0.0, 0.0, -0.1f));
		}
		if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
			camera->localMove(glm::vec3(0.1f, 0.0, 0.0));
		}

		if (key == GLFW_KEY_E && action == GLFW_REPEAT) {
			camera->localMove(glm::vec3(0.0, 0.1f, 0.0));
		}
		if (key == GLFW_KEY_Q && action == GLFW_REPEAT) {
			camera->localMove(glm::vec3(0.0, -0.1f, 0.0));
		}

		if (key == GLFW_KEY_K && action == GLFW_PRESS) {
			showMouse = !showMouse;
			if (!showMouse) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}



		if (key == GLFW_KEY_B && action == GLFW_PRESS) {
			lineVerts.push_back(camera->getPosition());

			prevPolyLine = std::make_shared<Lines>(lineVerts);
			renderer->createObject(prevPolyLine, shader);

			prevBezierLine = std::make_shared<Lines>(BezieBuilder::bezierBernstein(lineVerts));
			renderer->createObject(prevBezierLine, shader);

			if (lineVerts.size() >= 4) {
				std::vector<glm::vec3> lastFour(lineVerts.end() - 4, lineVerts.end());
				prevInterpLine = std::make_shared<Lines>(BezieBuilder::bezierInterpolated(lastFour));
				renderer->createObject(prevInterpLine, shader);
			}
		}

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			if (prevInterpLine != nullptr) {
				camera->startMoveOnCurve(prevInterpLine->getVertices());
			}
			else {
				camera->startMoveOnCurve(prevBezierLine->getVertices());
			}
		}
		if (key == GLFW_KEY_X && action == GLFW_PRESS) {
			if (controlledObject && spline) {
				std::cout << "Starting object movement along B-spline.\n";
				controlledObject->startMoveOnCurve(spline->getCurvePoints(), spline->getTangents());
			}
			else {
				std::cerr << "Error: Object or spline not set before pressing X.\n";
			}
		}
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		// Ensure the first call initializes the last position
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		// Calculate the change in mouse position (delta)
		float deltaX = xpos - lastX;
		float deltaY = lastY - ypos;  // Flip the Y axis for proper camera rotation

		// Update the camera's rotation based on the mouse movement
		camera->rotate(-deltaX * sensitivity, glm::vec3(0.0, 1.0, 0.0)/*camera->getUp()*/);  // Yaw (horizontal movement)
		camera->rotate(deltaY * sensitivity, camera->getRight());  // Pitch (vertical movement)

		// Store the current mouse position for the next frame
		lastX = xpos;
		lastY = ypos;
	}


	void setControlledObject(Object* obj) {
		controlledObject = obj;
	}

	void setSpline(Bspline* bs) {
		spline = bs;
	}

}

