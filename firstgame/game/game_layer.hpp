//
// Created by shepherd on 6.4.23.
//

#ifndef FIRSTGAME_GAME_LAYER_HPP
#define FIRSTGAME_GAME_LAYER_HPP

#include <chernoengine2/core/layer.hpp>
#include <chernoengine2/renderer/orthographic_camera.hpp>

#include <firstgame/game/level.hpp>

namespace firstgame {

class GameLayer : public chernoengine2::Layer {
public:
    GameLayer();

    void OnUpdate(float delta_time) override;

    void OnImguiRender() override;

private:
    void CreateCamera(float aspectRatio);

    chernoengine2::Scope<chernoengine2::OrthographicCamera> camera_;
    Level level_;
};

} // firstgame

#endif //FIRSTGAME_GAME_LAYER_HPP
