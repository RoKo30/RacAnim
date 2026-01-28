#pragma once

#include "Transform.h"
#include <windows.h>
#include <vector>
#include <iostream>
#include "Camera.h"
#include "Renderable.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"

class Object : public Transform {
protected:
    std::vector<std::shared_ptr<Renderable>> renderableMeshes;
    Shader* shader;
    Material* mat;
    Texture* tex;
    bool drawTriangles = true;

public:
    Object(std::shared_ptr<Renderable> mesh, Shader* shader, Material* mat, Texture* tex) : shader(shader), Transform(0), mat(mat), tex(tex) {
        if(mesh != nullptr)
            renderableMeshes.push_back(mesh);
    }

    Object(std::shared_ptr<Renderable> mesh, Shader* shader, Material* mat) : shader(shader), Transform(0), mat(mat) {
        if (mesh != nullptr)
            renderableMeshes.push_back(mesh);
    }

    Object(std::shared_ptr<Renderable> mesh, Shader* shader) : shader(shader), Transform(0) {
        if (mesh != nullptr)
            renderableMeshes.push_back(mesh);
    }

    void render(Camera& camera, glm::vec2 screenSize, Light& light) const {
        if (renderableMeshes.empty()) return;
        glUseProgram(shader->ID);

        if (tex != nullptr) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex->id);
            glUniform1i(glGetUniformLocation(shader->ID, "diffuseTexture"), 0);
        }
        // Light properties
        GLint lightPosLoc = glGetUniformLocation(shader->ID, "lightPosition");
        GLint lightAmbientLoc = glGetUniformLocation(shader->ID, "lightAmbient");
        GLint lightDiffuseLoc = glGetUniformLocation(shader->ID, "lightDiffuse");
        GLint lightSpecularLoc = glGetUniformLocation(shader->ID, "lightSpecular");

        glm::vec3 lightPos = light.getPosition();
        glm::vec3 lightAmbient = light.getAmbientIntensity();
        glm::vec3 lightDiffuse = light.getDiffuseIntensity();
        glm::vec3 lightSpecular = light.getSpecularIntensity();

        glUniform3fv(lightPosLoc, 1, &lightPos[0]);
        glUniform3fv(lightAmbientLoc, 1, &lightAmbient[0]);
        glUniform3fv(lightDiffuseLoc, 1, &lightDiffuse[0]);
        glUniform3fv(lightSpecularLoc, 1, &lightSpecular[0]);

        // Material uniforms
        if (mat != nullptr) {
            GLint aLoc = glGetUniformLocation(shader->ID, "materialAmbient");
            GLint dLoc = glGetUniformLocation(shader->ID, "materialDiffuse");
            GLint sLoc = glGetUniformLocation(shader->ID, "materialSpecular");
            GLint shLoc = glGetUniformLocation(shader->ID, "materialShininess");

            glUniform3fv(aLoc, 1, &mat->getAmbient()[0]);
            glUniform3fv(dLoc, 1, &mat->getDiffuse()[0]);
            glUniform3fv(sLoc, 1, &mat->getSpecular()[0]);
            glUniform1f(shLoc, mat->getShininess());
        }

        // Matrices
        glm::mat4 model = getModelMatrix();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getPerspectiveMatrix(screenSize);

        GLint modelLoc = glGetUniformLocation(shader->ID, "tModelMatrix");
        GLint viewLoc = glGetUniformLocation(shader->ID, "tViewMatrix");
        GLint projLoc = glGetUniformLocation(shader->ID, "tPerspectiveMatrix");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        // Draw
        for (const auto& renderableMesh : renderableMeshes) {
            renderableMesh->draw(drawTriangles);
        }
    }

     
    void drawLines() {
        drawTriangles = true; 
    }

    Shader* getShader() {
        return shader;
    }

    Material* getMaterial() {
        return mat;
    }

    Texture* getTexture() {
        return tex;
    }

    std::shared_ptr<Renderable> getMesh() {
        return renderableMeshes[0];
    }


    /*~Object() {
        delete shader;
    }*/

};

