//
// Created by shepherd on 6.4.23.
//

#ifndef FIRSTGAME_PLAYER_HPP
#define FIRSTGAME_PLAYER_HPP

#include <glm/glm.hpp>
#include <chernoengine2/renderer/texture.hpp>

#include <firstgame/game/particle_system.hpp>

namespace firstgame {

class Player {
public:
    Player();

    void OnUpdate(float delta_time);

    void OnRender();

    float GetRotation() const;

    const glm::vec2& GetPosition() const;

    void Reset();

    int GetScore() const;

private:
    chernoengine2::Ref<chernoengine2::Texture2D> ship_texture_;
    glm::vec2 position_ = {-10.0f, 0.0f};
    float speed_x_ = 9.0f;
    glm::vec2 velocity_ = {speed_x_, 0.0f};

    float engine_power_ = 0.5f;
    float gravity_ = 0.4f;

    float time_ = 0.0f;
    float smoke_emit_interval_ = 0.4f;
    float smoke_emit_next_interval_ = smoke_emit_interval_;

    ParticleSystem particle_system_;
    ParticleProps smoke_particle_, engine_particle_;
};

} // firstgame

#endif //FIRSTGAME_PLAYER_HPP
