#include <iostream>

#include <chernoengine2/core/application.hpp>

#include <firstgame/game/game_layer.hpp>

int main() {
    chernoengine2::Application app;
    app.PushLayer(new firstgame::GameLayer);
    app.Run();
}
