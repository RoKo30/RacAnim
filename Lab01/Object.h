/*#pragma once

#include "Transform.h"
#include <windows.h>
#include <vector>
#include <iostream>
#include "Camera.h"
#include "Renderable.h"
#include "Shader.h"

class Object : public Transform {
private:
	std::vector<std::shared_ptr<Renderable>> renderableMeshes;
	Shader *shader;
	bool drawTriangles = true;

public:
	Object(std::shared_ptr<Renderable> mesh, Shader *shader) : shader(shader), Transform(0) {
		renderableMeshes.push_back(mesh);
	}


	void render(Camera& camera, glm::vec2 screenSize) const {
		//std::cout << camera.getPosition().x << "\n";
		glUseProgram(shader->ID);

		// Get transformation matrices
		glm::mat4 model = getModelMatrix();
		glm::mat4 view = camera.getViewMatrix();           
		glm::mat4 projection = camera.getPerspectiveMatrix(screenSize);

		// Send matrices to shader
		GLint modelLoc = glGetUniformLocation(shader->ID, "tModelMatrix");
		GLint viewLoc = glGetUniformLocation(shader->ID, "tViewMatrix");
		GLint projLoc = glGetUniformLocation(shader->ID, "tPerspectiveMatrix");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

		for (const auto renderableMesh : renderableMeshes) {
			renderableMesh->draw((drawTriangles) ? true : false);
		}
	}

	void drawLines() {
		drawTriangles = false;
	}


	/*~Object() {
		delete shader;
	}*/
//};*/

#pragma once

#include "Transform.h"
#include <windows.h>
#include <vector>
#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Renderable.h"
#include "Shader.h"

class Object : public Transform {
private:
    std::vector<std::shared_ptr<Renderable>> renderableMeshes;
    Shader* shader;
    bool drawTriangles = true;

    // Movement along spline
    std::vector<glm::vec3> path;  // B-spline curve points
    std::vector<glm::vec3> tangents;  // B-spline curve points
    size_t pathIndex = 0;
    bool moving = false;
    float moveDelay = 0.02f;  // seconds between steps
    double lastMoveTime = 0.0;

public:
    Object(std::shared_ptr<Renderable> mesh, Shader* shader)
        : shader(shader), Transform(0) {
        renderableMeshes.push_back(mesh);
    }

    // --------------------------------------------------------
    //  Rendering
    // --------------------------------------------------------
    void render(Camera& camera, glm::vec2 screenSize) const {
        glUseProgram(shader->ID);

        glm::mat4 model = getModelMatrix();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getPerspectiveMatrix(screenSize);

        GLint modelLoc = glGetUniformLocation(shader->ID, "tModelMatrix");
        GLint viewLoc = glGetUniformLocation(shader->ID, "tViewMatrix");
        GLint projLoc = glGetUniformLocation(shader->ID, "tPerspectiveMatrix");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        for (const auto& renderableMesh : renderableMeshes)
            renderableMesh->draw(drawTriangles);
    }

    void drawLines() { drawTriangles = false; }

    // --------------------------------------------------------
    //  B-spline path movement
    // --------------------------------------------------------
    void startMoveOnCurve(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& tangents, float delay = 0.02f) {
        if (vertices.empty()) {
            std::cerr << "Warning: empty curve path.\n";
            return;
        }

        path = vertices;
        this->tangents = tangents;
        pathIndex = 0;
        moveDelay = delay;
        moving = true;
        lastMoveTime = glfwGetTime();

        // Move immediately to first position
        setPosition(path.front());
    }

    void updateMovement() {
        if (!moving || pathIndex >= path.size()) return;

        double currentTime = glfwGetTime();
        if (currentTime - lastMoveTime >= moveDelay) {
            // Move to next point on path
            setPosition(path[pathIndex]);

            // --- Rotation along spline ---
            if (pathIndex < tangents.size()) {
                glm::vec3 newFront = glm::normalize(tangents[pathIndex]);

                // Option 1: Directly set orientation (simple & stable)
               // glm::vec3 axis = glm::normalize(glm::cross(this->getFront(), newFront));
               // float len = glm::length(axis);
                //if (len > 1e-6f) {  // only rotate if axis is valid
                  //  axis = axis / len;
                   // float angle = acos(glm::clamp(glm::dot(this->getFront(), newFront), -1.0f, 1.0f));
                   // rotate(angle, axis);
                    setOrientation(newFront);
               // }

                // Option 2: Smooth incremental rotation (optional)
                // glm::vec3 axis = glm::normalize(glm::cross(front, newFront));
                // float angle = acos(glm::clamp(glm::dot(front, newFront), -1.0f, 1.0f));
                // if (!isnan(angle) && glm::length(axis) > 0.0001f)
                //     rotate(angle, axis);
            }

            pathIndex++;
            lastMoveTime = currentTime;

            if (pathIndex >= path.size()) {
                moving = false; // reached end of curve
            }
        }
    }


    bool isMoving() const { return moving; }
};
