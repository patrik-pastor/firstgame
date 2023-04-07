//
// Created by shepherd on 7.4.23.
//

#include <firstgame/game/particle_system.hpp>

#include <glm/gtx/compatibility.hpp>
#include <chernoengine2/renderer/renderer_2d.hpp>

#include <firstgame/game/random.hpp>

namespace firstgame {

ParticleSystem::ParticleSystem() {
    particle_pool_.resize(1000);
}

void ParticleSystem::Emit(const ParticleProps& particle_props) {
    Particle& particle = particle_pool_[pool_index_];
    particle.active = true;
    particle.position = particle_props.position;
    particle.rotation = Random::Get() * 2.0f * glm::pi<float>();

    // velocity
    particle.velocity = particle_props.velocity;
    particle.velocity.x += particle_props.velocity_variation.x * (Random::Get() - 0.5f);
    particle.velocity.y += particle_props.velocity_variation.y * (Random::Get() - 0.5f);

    // color
    particle.color_begin = particle_props.color_begin;
    particle.color_end = particle_props.color_begin;

    // size
    particle.size_begin = particle_props.size_begin + particle_props.size_variation * (Random::Get() - 0.5f);
    particle.size_end = particle_props.size_end;

    // life
    particle.life_time = particle_props.life_time;
    particle.life_remaining = particle_props.life_time;

    pool_index_ = --pool_index_ % particle_pool_.size();
}

void ParticleSystem::OnUpdate(float delta_time) {
    for (auto& particle: particle_pool_) {
        if(!particle.active){
            continue;
        }
        if(particle.life_remaining <= 0.0f){
            particle.active = false;
            continue;
        }
        particle.life_remaining -= delta_time;
        particle.position += particle.velocity * delta_time;
        particle.rotation += 0.01f * delta_time;
    }
}

void ParticleSystem::OnRender() {
    for(auto& particle : particle_pool_){
        if(!particle.active){
            continue;
        }
        float life = particle.life_remaining / particle.life_time;
        glm::vec4 color = glm::lerp(particle.color_end, particle.color_begin, life);
        color.a = color.a * life;

        float size = glm::lerp(particle.size_end, particle.size_begin, life);
        chernoengine2::Renderer2D::DrawRotatedQuad(particle.position, {size, size}, particle.rotation, color);
    }
}

} // firstgame