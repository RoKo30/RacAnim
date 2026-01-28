#pragma once

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <cassert>
#include "Lines.h"


class Bspline : public Lines{
private:
    std::vector<glm::vec3> poligon;   
    std::vector<glm::vec3> B_Spline;  
    std::vector<glm::vec3> tangents;  


public:
    Bspline(const aiScene* scene, float dt = 0.05f) : Lines() {
        if (!scene || !scene->HasMeshes()) {
            std::cerr << "Error: Invalid or empty Assimp scene.\n";
            return;
        }

        aiMesh* mesh = scene->mMeshes[0];
        poligon.reserve(mesh->mNumVertices);
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            aiVector3D v = mesh->mVertices[i];
            poligon.emplace_back(v.x, v.y, v.z);
        }

        BuildB_spline(dt);
        setVertecies(B_Spline);

    }

    Bspline(const std::vector<glm::vec3>& ctrlPts, float dt = 0.05f)
        : poligon(ctrlPts) {
        BuildB_spline(dt);
    }

    void BuildB_spline(float dt = 0.05f) {
        B_Spline.clear();

        const size_t m = poligon.size();
        if (m < 4) {
            std::cerr << "Not enough control points for cubic B-spline (need >=4).\n";
            return;
        }

        glm::mat4 M(
            -1.0f, 3.0f, -3.0f, 1.0f,  
            3.0f, -6.0f, 0.0f, 4.0f,
            -3.0f, 3.0f, 3.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 0.0f
        );

        glm::mat4 M_deriv(
            -1, 2, -1, 0,
            3, -4, 0, 0,
            -3, 2 ,1, 0,
            1, 0, 0, 0
        );

        M *= (1.0f / 6.0f);
        M_deriv *= (1.0f / 2.0f);


        for (size_t i = 1; i <= m - 3; ++i) {
            for (float t = 0.0f; t <= 1.0f; t += dt) {
                float t2 = t * t;
                float t3 = t2 * t;
                glm::vec4 T(t3, t2, t, 1.0f);   
                glm::vec4 T_deriv(t2, t, 1, 0);

                glm::vec4 c = T * M; 
                glm::vec4 c1 = T_deriv * M_deriv;

                glm::vec3 p(0.0f);
                p += c.x * poligon[i - 1];
                p += c.y * poligon[i];
                p += c.z * poligon[i + 1];
                p += c.w * poligon[i + 2];
                B_Spline.push_back(p);

                glm::vec3 p1(0.0f);
                p1 += c1.x * poligon[i - 1];
                p1 += c1.y * poligon[i];
                p1 += c1.z * poligon[i + 1];
                p1 += c1.w * poligon[i + 2];
                tangents.push_back(p1);
            }
        }


        std::cout << "B-spline built: control points = " << m
            << ", sampled points = " << B_Spline.size() << std::endl;
    }

    const std::vector<glm::vec3>& getControlPoints() const { return poligon; }
    const std::vector<glm::vec3>& getCurvePoints()   const { return B_Spline; }
    const std::vector<glm::vec3>& getTangents()   const { return tangents; }

};
