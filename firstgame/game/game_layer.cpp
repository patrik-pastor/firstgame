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

void GameLayer::OnUpdate(float delta_time) {
    level_.OnUpdate(delta_time);

    const auto& player_position = level_.GetPlayer().GetPosition();
    camera_->SetPosition({player_position.x, player_position.y, 0.0});

    chernoengine2::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    chernoengine2::RenderCommand::Clear();

    chernoengine2::Renderer2D::BeginScene(*camera_);
    level_.OnRender();
    chernoengine2::Renderer2D::EndScene();
}

void GameLayer::OnImguiRender() {
    int player_score = level_.GetPlayer().GetScore();
    std::string score_string = "Score: " + std::to_string(player_score);
    ImGui::GetForegroundDrawList()->AddText(ImGui::GetWindowPos(), 0xffffffff, score_string.c_str());
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