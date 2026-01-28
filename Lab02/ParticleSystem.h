#pragma once
#include <vector>
#include <cstdlib>
#include "Transform.h"
#include "Particle.h"

class ParticleSystem : public Transform {
private:
    std::vector<Particle> particles;
    std::vector<std::unique_ptr<Material>> particleMaterials;

public:
ParticleSystem(int count,std::shared_ptr<Renderable> quad,Shader* shader,Material* mat,Texture* tex) : Transform(0) {
        particles.reserve(count);
        particleMaterials.reserve(count);

        for (int i = 0; i < count; i++) {
            particleMaterials.push_back(std::make_unique<Material>(*mat));

            particles.emplace_back(
                quad,
                shader,
                particleMaterials.back().get(),
                tex
            );
        }
    }


    void update(float dt, const Camera& camera) {
        for (auto& p : particles) {
            if (!p.isAlive())
                respawnParticle(p);
            p.update(dt, camera);
        }
    }

    void render(Renderer& renderer) {
        for (auto& p : particles)
            renderer.addObject(p);
    }

private:
    void respawnParticle(Particle& p) {
        glm::vec3 randomVel(
            ((rand() % 100) / 100.0f - 0.5f) * 2.0f,
            ((rand() % 100) / 100.0f) * 3.0f,
            ((rand() % 100) / 100.0f - 0.5f) * 2.0f
        );

        p.respawn(
            getPosition(),
            randomVel,
            5.0f + (rand() % 7)
        );
    }
};
