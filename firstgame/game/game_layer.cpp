//
// Created by shepherd on 6.4.23.
//

#include <firstgame/game/game_layer.hpp>

#include <imgui/imgui.h>
#include <chernoengine2/core/application.hpp>
#include <chernoengine2/renderer/renderer_2d.hpp>

#include <firstgame/game/random.hpp>

namespace firstgame {

GameLayer::GameLayer() : chernoengine2::Layer("GameLayer") {
    auto& window = chernoengine2::Application::GetInstance().GetWindow();
    float aspectRatio = (float) window.GetWidth() / window.GetHeight();
    CreateCamera(aspectRatio);
    Random::Init();
}

void GameLayer::OnAttach() {
    ImGuiIO& io = ImGui::GetIO();
    font_ = io.Fonts->AddFontFromFileTTF("assets/opensans-regular.ttf", 120.0f);
}

void GameLayer::OnUpdate(float delta_time) {
    time_ += delta_time;
    if ((int) (time_ * 10.0f) % 8 > 4) {
        blink_ = !blink_;
    }
    if (level_.IsGameOver()) {
        game_state_ = GameState::GAMEOVER;
    }
    if (game_state_ == GameState::PLAY) {
        level_.OnUpdate(delta_time);
    }

    const auto& player_position = level_.GetPlayer().GetPosition();
    camera_->SetPosition({player_position.x, player_position.y, 0.0});

    // Renderer
    chernoengine2::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    chernoengine2::RenderCommand::Clear();

    chernoengine2::Renderer2D::BeginScene(*camera_);
    level_.OnRender();
    chernoengine2::Renderer2D::EndScene();
}

void GameLayer::OnImguiRender() {
    switch (game_state_) {
        case GameState::PLAY: {
            int player_score = level_.GetPlayer().GetScore();
            std::string score_string = "Score: " + std::to_string(player_score);
            ImGui::GetForegroundDrawList()->AddText(ImGui::GetWindowPos(), 0xffffffff, score_string.c_str());
            break;
        }
        case GameState::MAINMENU: {
            auto pos = ImGui::GetWindowPos();
            int width = chernoengine2::Application::GetInstance().GetWindow().GetWidth();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (blink_) {
                ImGui::GetForegroundDrawList()->AddText(font_, 120.0f, pos, 0xffffffff, "Click to Play!");
            }
            break;
        }
        case GameState::GAMEOVER: {
            auto pos = ImGui::GetWindowPos();
            auto width = chernoengine2::Application::GetInstance().GetWindow().GetWidth();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (blink_)
                ImGui::GetForegroundDrawList()->AddText(font_, 120.0f, pos, 0xffffffff, "Click to Play!");

            pos.x += 200.0f;
            pos.y += 150.0f;
            int playerScore = level_.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(font_, 48.0f, pos, 0xffffffff, scoreStr.c_str());
            break;
        }
    }
}

void GameLayer::OnEvent(chernoengine2::Event& event) {
    chernoengine2::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<chernoengine2::WindowResizeEvent>(BIND_EVENT_FN(GameLayer::OnWindowResize));
    dispatcher.Dispatch<chernoengine2::MouseButtonPressedEvent>(BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

bool GameLayer::OnMouseButtonPressed(chernoengine2::MouseButtonPressedEvent& e) {
    if (game_state_ == GameState::GAMEOVER) {
        level_.Reset();
    }
    game_state_ = GameState::PLAY;
    return false;
}

bool GameLayer::OnWindowResize(chernoengine2::WindowResizeEvent& e) {
    CreateCamera((float) e.GetWidth() / e.GetHeight());
    return false;
}

void GameLayer::CreateCamera(float aspectRatio) {
    float magnitude = 8.0f;
    float bottom = -magnitude;
    float top = magnitude;
    float left = bottom * aspectRatio;
    float right = top * aspectRatio;
    camera_ = chernoengine2::CreateScope<chernoengine2::OrthographicCamera>(left, right, bottom, top);
}


} // firstgame