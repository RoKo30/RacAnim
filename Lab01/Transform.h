#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "MyGLM.h"


class Transform {
protected:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 scale;

public:
	Transform(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 right, glm::vec3 scale)
		: position(position), front(front), up(up), right(right), scale(scale) {
	};

	Transform(int zero) {
		this->position = glm::vec3(0.0);
		this->front = glm::vec3(0.0, 0.0, -1.0);
		this->up = glm::vec3(0.0, 1.0, 0.0);
		this->right = glm::normalize(glm::cross(front, up));
		this->scale = glm::vec3(1.0);
	};

	glm::mat4 getModelMatrix() const {
		glm::mat4 model = glm::mat4(1.0f);
		model = model * MyGLM::translate3D(position);

		// orientation basis
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0] = glm::vec4(right, 0.0f);
		rotation[1] = glm::vec4(up, 0.0f);
		rotation[2] = glm::vec4(-front, 0.0f);
		model *= rotation;
		model = model * MyGLM::scale3D(scale);

		return model;
	}

	glm::mat4 getViewMatrix() const {
		return MyGLM::lookAtMatrix(position, position + front, up);
	}

	void rotate(float angleDegrees, glm::vec3 axis) {
		glm::mat4 rotMat = MyGLM::rotate3D(axis, angleDegrees);
		front = glm::normalize(glm::vec3(rotMat * glm::vec4(front, 0.0f)));
		up = glm::normalize(glm::vec3(rotMat * glm::vec4(up, 0.0f)));
		right = glm::normalize(glm::cross(front, up));
		up = glm::normalize(glm::cross(right, front));
	}



	void globalMove(glm::vec3 delta) {
		position += delta;
	}

	void localMove(glm::vec3 delta) {
		position += right * delta.x;
		position += up * delta.y;
		position += front * delta.z;

		//std::cout << "POSITION " << "x: " << position.x << ", y: " << position.y << ", z: " << position.z << "\n";
		//std::cout << "FRONT " << "x: " << front.x << ", y: " << front.y << ", z: " << front.z << "\n";
	}

	void setOrientation(glm::vec3 newFront, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f)) {
		front = glm::normalize(newFront);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	void setPosition(glm::vec3 newPosition) {
		position = newPosition;
	}

	glm::vec3 getPosition() {
		return position;
	}

	void setScale(glm::vec3 newScale) {
		scale = newScale;
	}

	glm::vec3 getRight() {
		return right;
	}

	glm::vec3 getUp() {
		return up;
	}

	glm::vec3 getFront() {
		return front;
	}
};