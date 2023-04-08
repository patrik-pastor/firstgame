//
// Created by shepherd on 6.4.23.
//

#ifndef FIRSTGAME_GAME_LAYER_HPP
#define FIRSTGAME_GAME_LAYER_HPP

#include <imgui/imgui.h>
#include <chernoengine2/core/layer.hpp>
#include <chernoengine2/renderer/orthographic_camera.hpp>
#include <chernoengine2/events/application_event.hpp>
#include <chernoengine2/events/mouse_event.hpp>

#include <firstgame/game/level.hpp>

namespace firstgame {

class GameLayer : public chernoengine2::Layer {
public:
    GameLayer();

    void OnAttach() override;

    void OnUpdate(float delta_time) override;

    void OnImguiRender() override;

    void OnEvent(chernoengine2::Event &event) override;

    bool OnMouseButtonPressed(chernoengine2::MouseButtonPressedEvent& e);

    bool OnWindowResize(chernoengine2::WindowResizeEvent& e);

private:
    enum class GameState {
        PLAY, MAINMENU, GAMEOVER
    };

    void CreateCamera(float aspectRatio);

    chernoengine2::Scope<chernoengine2::OrthographicCamera> camera_;
    Level level_;
    ImFont *font_;
    float time_ = 0.0f;
    bool blink_ = false;
    GameState game_state_ = GameState::MAINMENU;
};

} // firstgame

#endif //FIRSTGAME_GAME_LAYER_HPP
