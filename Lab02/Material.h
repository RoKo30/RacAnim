#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Material {
	glm::vec3 aCoef = glm::vec3(0.0);
	glm::vec3 dCoef = glm::vec3(0.0);
	glm::vec3 sCoef = glm::vec3(0.0);
	double rN;


public:
	Material(const Material&) = default;
	Material(glm::vec3 aCoef, glm::vec3 dCoef, glm::vec3 sCoef, double rn) : aCoef(aCoef), dCoef(dCoef), sCoef(sCoef), rN(rn){}
	Material() : aCoef(1.0), dCoef(1.0), sCoef(1.0), rN(1.0) {}
	Material(const aiScene* scene, int i) {
		if (scene->HasMaterials()) {
			std::cout << "materijal[" << i << "]:\n";

			aiString naziv;
			std::cout << "naziv: ";
			scene->mMaterials[i]->Get(AI_MATKEY_NAME, naziv);
			std::cout << naziv.C_Str() << std::endl;

			glm::vec3 ambientColor;
			aiColor3D ambientK, diffuseK, specularK, reflectiveK, emissiveK;
			float shininessK;

			std::cout << "ambient: ";
			scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
			aCoef.x = ambientK.r;
			aCoef.y = ambientK.g;
			aCoef.z = ambientK.b;

			std::cout << ambientK.r << " " << ambientK.g << " " << ambientK.b << std::endl;

			std::cout << "diffuse: ";
			scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
			dCoef.x = diffuseK.r;
			dCoef.y = diffuseK.g;
			dCoef.z = diffuseK.b;
			std::cout << diffuseK.r << " " << diffuseK.g << " " << diffuseK.b << std::endl;

			std::cout << "specular: ";
			scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularK);
			sCoef.x = specularK.r;
			sCoef.y = specularK.g;
			sCoef.z = specularK.b;
			std::cout << specularK.r << " " << specularK.g << " " << specularK.b << std::endl;

			std::cout << "shininess ";
			scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininessK);
			rN = shininessK;
			std::cout << shininessK << std::endl;

			std::cout << "____________________________________________________" << std::endl;
			
		}
	}

	const glm::vec3& getAmbient() const { return aCoef; }
	const glm::vec3& getDiffuse() const { return dCoef; }
	const glm::vec3& getSpecular() const { return sCoef; }

	void setColor(glm::vec3 a, glm::vec3 d, glm::vec3 s) {
		aCoef = a;
		dCoef = d;
		sCoef = s;
	}

	float getShininess() const { return static_cast<float>(rN); }
};