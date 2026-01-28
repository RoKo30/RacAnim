#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include "Object.h"

class Particle : public Object {
private:
	double currentTime = 10.0;
    double maxLifeTime = 10.0;
    glm::vec3 velocity = glm::vec3(1.0);

public:
    Particle(std::shared_ptr<Renderable> mesh, Shader* shader, Material* mat, Texture* tex) : Object(mesh, shader, mat, tex), velocity(0.0), currentTime(0.0), maxLifeTime(10.0) {}

    bool isAlive() const {
        return currentTime > 0.0f;
    }

    void respawn(const glm::vec3& position, const glm::vec3& vel, float lifetime) {
        setPosition(position);
        velocity = vel;
        currentTime = lifetime;
        maxLifeTime = lifetime;
    }

    void update(float dt, const Camera& camera) {
        if (currentTime <= 0.0) return;
        currentTime -= dt;

        float lifeRatio = currentTime / maxLifeTime; 
        float size = glm::clamp(lifeRatio, 0.1f, 1.0f);
        setScale(glm::vec3(size * 0.2f));
        if (getMaterial()) {
            getMaterial()->setColor(
                glm::vec3(lifeRatio, 0.5, 0.5),
                glm::vec3(lifeRatio, 0.5, 0.5),
                glm::vec3(lifeRatio, 0.5, 0.5)
            );
        }

        globalMove(velocity * dt);

        setBillboard(
            camera.getRight(),
            camera.getUp(),
            camera.getFront()
        );

    }

};

