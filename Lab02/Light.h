#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <chrono>
#include "Transform.h"
#include "Lines.h"

class Light : public Transform {
	glm::vec3 aIntensity;
	glm::vec3 dIntensity;
	glm::vec3 sIntensity;


public:
	Light(glm::vec3 aI, glm::vec3 dI, glm::vec3 sI) : Transform(0) {
		aIntensity = aI;
		dIntensity = dI;
		sIntensity = sI;
	}
	Light() : Transform(0) {
		aIntensity = glm::vec3(1.0, 1.0, 1.0); 
		dIntensity = glm::vec3(1.0, 1.0, 1.0);
		sIntensity = glm::vec3(1.0, 1.0, 1.0);
	}

	glm::vec3 getAmbientIntensity() const { return aIntensity; }
	glm::vec3 getDiffuseIntensity() const { return dIntensity; }
	glm::vec3 getSpecularIntensity() const { return sIntensity; }
};