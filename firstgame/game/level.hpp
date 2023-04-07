//
// Created by shepherd on 6.4.23.
//

#ifndef FIRSTGAME_LEVEL_HPP
#define FIRSTGAME_LEVEL_HPP

#include <chernoengine2/renderer/texture.hpp>

#include <firstgame/game/player.hpp>

namespace firstgame {

struct Pillar {
    glm::vec3 top_position = {0.0, 10.0f, 0.0f};
    glm::vec2 top_scale = {15.0f, 15.0f};

    glm::vec3 bottom_position = {10.0f, 10.0f, 0.0f};
    glm::vec2 bottom_scale = {15.0f, 15.0f};
};

class Level {
public:
    Level();

    void OnUpdate(float delta_time);

    void OnRender();

    const Player& GetPlayer() const;

private:
    void CreatePillar(int index, float offset);

    bool CollisionTest();

    void GameOver();

    Player player_;
    chernoengine2::Ref<chernoengine2::Texture2D> triangle_texture;
    glm::vec3 pillar_hsv_ = {0.0f, 0.8f, 0.8f};
    std::vector<Pillar> pillars_;
    int pillar_index_ = 0;
    float pillar_target_ = 30.0f;
};

} // firstgame

#endif //FIRSTGAME_LEVEL_HPP
