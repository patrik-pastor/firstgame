//
// Created by shepherd on 7.4.23.
//

#ifndef FIRSTGAME_PARTICLE_SYSTEM_HPP
#define FIRSTGAME_PARTICLE_SYSTEM_HPP

#include <vector>

#include <glm/glm.hpp>

namespace firstgame {

struct ParticleProps {
    glm::vec2 position;
    glm::vec2 velocity, velocity_variation;
    glm::vec4 color_begin, color_end;
    float size_begin, size_end, size_variation;
    float life_time = 1.0f;
};

class ParticleSystem {
public:
    ParticleSystem();
    void Emit(const ParticleProps& particle_props);
    void OnUpdate(float delta_time);
    void OnRender();

private:
    struct Particle {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 color_begin, color_end;
        float rotation = 0.0f;
        float size_begin, size_end;
        float life_time = 1.0f;
        float life_remaining = 0.0f;
        bool active = false;
    };
    std::vector<Particle> particle_pool_;
    int pool_index_ = 999;
};

} // firstgame

#endif //FIRSTGAME_PARTICLE_SYSTEM_HPP
