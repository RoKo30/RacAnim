#pragma once

#include "Object.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include <vector>

class Renderer {
	std::vector<Object*> objects;
	Camera camera;
	Light light;
	glm::vec2 screenSize;

public:
	Renderer(glm::vec2 screensSize, float zoom) : screenSize(screenSize), camera(zoom), light() {}

	void render() {
		for (Object* object : objects) {
			if (object)
				object->render(camera, screenSize, light);
		}
	}

	void addObject(Object& o) {
		objects.push_back(&o);
	}

	void clearObjects() { 
		objects.clear(); 
	}


	Camera* getCamera() {
		return &camera;
	}

	Light* getLight() {
		return &light;
	}

};

