#pragma once

#include "Object.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include <vector>

class Renderer {
	std::vector<Object> objects;
	Camera camera;
	glm::vec2 screenSize;

public:
	Renderer(glm::vec2 screensSize, float zoom) : screenSize(screenSize), camera(zoom) {}

	void render() {
		for (const Object& object : objects) {
			object.render(camera, screenSize);
		}
	}
	void createObject(std::shared_ptr<Renderable> mesh, Shader *shader) {
		objects.push_back(Object(mesh, shader));
	}

	Camera* getCamera() {
		return &camera;
	}

	std::vector<Object>& getObjects() {
		return objects;
	}


};

