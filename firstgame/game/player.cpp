//
// Created by shepherd on 6.4.23.
//

#include <firstgame/game/player.hpp>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <chernoengine2/renderer/renderer_2d.hpp>
#include <chernoengine2/core/input.hpp>
#include <chernoengine2/core/key_codes.hpp>

namespace firstgame {

Player::Player() {
    ship_texture_ = chernoengine2::Texture2D::Create("assets/textures/ship.png");

    // smoke
    smoke_particle_.position = {0.0f, 0.0f};
    smoke_particle_.velocity = {-2.0f, 0.0f};
    smoke_particle_.velocity_variation = {4.0f, 2.0f};
    smoke_particle_.size_begin = 0.35f;
    smoke_particle_.size_end = 0.0f;
    smoke_particle_.size_variation = 0.15f;
    smoke_particle_.color_begin = {0.8f, 0.8f, 0.8f, 1.0f};
    smoke_particle_.color_end = {0.6f, 0.6f, 0.6f, 1.0f};
    smoke_particle_.life_time = 4.0f;

    // flames
    engine_particle_.position = {0.0f, 0.0f};
    engine_particle_.velocity = {-2.0f, 0.0f};
    engine_particle_.velocity_variation = {3.0f, 1.0f};
    engine_particle_.size_begin = 0.5f;
    engine_particle_.size_end = 0.0f;
    engine_particle_.size_variation = 0.3f;
    engine_particle_.color_begin = {254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f};
    engine_particle_.color_end = {254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f};
    engine_particle_.life_time = 1.0f;
}

void Player::OnUpdate(float delta_time) {
    time_ += delta_time;

    if (chernoengine2::Input::IsKeyPressed(KEY_SPACE)) {
        velocity_.y += engine_power_;
        if (velocity_.y < 0.0f) {
            velocity_.y += engine_power_ * 2.0f;
        }
        // flames
        glm::vec2 emission_point = {0.0f, -0.6f};
        float rotation = glm::radians(GetRotation());
        glm::vec4 rotated = glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
                            * glm::vec4(emission_point, 0.0f, 1.0f);
        engine_particle_.position = position_ + glm::vec2{rotated.x, rotated.y};
        engine_particle_.velocity.y = -velocity_.y * 0.2f - 0.2f;
        particle_system_.Emit(engine_particle_);
    } else {
        velocity_.y -= gravity_;
    }
    velocity_ = glm::clamp(velocity_, -20.0f, 20.0f);
    position_ += velocity_ * delta_time;

    // particles
    if (time_ > smoke_emit_next_interval_) {
        smoke_particle_.position = position_;
        particle_system_.Emit(smoke_particle_);
        smoke_emit_next_interval_ += smoke_emit_interval_;
    }
    particle_system_.OnUpdate(delta_time);
}

void Player::OnRender() {
    particle_system_.OnRender();
    chernoengine2::Renderer2D::DrawRotatedQuad({position_.x, position_.y, 0.5f}, {1.0f, 1.3f},
                                               glm::radians(GetRotation()), ship_texture_);
}

float Player::GetRotation() const {
    return velocity_.y * 4.0f - 90.0f;
}

const glm::vec2& Player::GetPosition() const {
    return position_;
}

void Player::Reset() {
    position_ = {-10.0f, 0.0f};
    velocity_ = {9.0f, 0.0f};
}

int Player::GetScore() const {
    return (position_.x + 10) / 10;
}


} // firstgame