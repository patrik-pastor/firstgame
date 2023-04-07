//
// Created by shepherd on 6.4.23.
//

#include <firstgame/game/level.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <chernoengine2/renderer/renderer_2d.hpp>

#include <firstgame/game/color.hpp>
#include <firstgame/game/random.hpp>

namespace firstgame {

static glm::vec4 hsv2rgb(const glm::vec3& hsv) {
    int H = (int) (hsv.x * 360.0f);
    double S = hsv.y;
    double V = hsv.z;

    double C = S * V;
    double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    double m = V - C;
    double Rs, Gs, Bs;

    if (H >= 0 && H < 60) {
        Rs = C;
        Gs = X;
        Bs = 0;
    } else if (H >= 60 && H < 120) {
        Rs = X;
        Gs = C;
        Bs = 0;
    } else if (H >= 120 && H < 180) {
        Rs = 0;
        Gs = C;
        Bs = X;
    } else if (H >= 180 && H < 240) {
        Rs = 0;
        Gs = X;
        Bs = C;
    } else if (H >= 240 && H < 300) {
        Rs = X;
        Gs = 0;
        Bs = C;
    } else {
        Rs = C;
        Gs = 0;
        Bs = X;
    }

    return {(Rs + m), (Gs + m), (Bs + m), 1.0f};
}

static bool PointInTri(const glm::vec2& p, glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) {
    float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
    float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

    if ((s < 0) != (t < 0))
        return false;

    float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

    return A < 0 ?
           (s <= 0 && s + t >= A) :
           (s >= 0 && s + t <= A);
}

Level::Level() {
    triangle_texture = chernoengine2::Texture2D::Create("assets/textures/triangle.png");
    pillars_.resize(5);
    for (int i = 0; i < 5; i++) {
        CreatePillar(i, i * 10.0f);
    }
}

void Level::OnUpdate(float delta_time) {
    player_.OnUpdate(delta_time);

    if (CollisionTest()) {
        GameOver();
        return;
    }

    pillar_hsv_.x += 0.1f * delta_time;
    if (pillar_hsv_.x > 1.0f) {
        pillar_hsv_.x = 0.0f;
    }

    if (player_.GetPosition().x > pillar_target_) {
        CreatePillar(pillar_index_, pillar_target_ + 20.0f);
        pillar_index_ = ++pillar_index_ % pillars_.size();
        pillar_target_ += 10.0f;
    }
}

void Level::OnRender() {
    const auto& player_pos = player_.GetPosition();

    glm::vec4 color = hsv2rgb(pillar_hsv_);

    // Background
    chernoengine2::Renderer2D::DrawQuad({player_pos.x, 0.0f, -0.8f}, {50.0f, 50.0f}, {0.3f, 0.3f, 0.3f, 1.0f});

    // Floor and ceiling
    chernoengine2::Renderer2D::DrawQuad({player_pos.x, 34.0f}, {50.0f, 50.0f}, color);
    chernoengine2::Renderer2D::DrawQuad({player_pos.x, -34.0f}, {50.0f, 50.0f}, color);

    for (auto& pillar: pillars_) {
        chernoengine2::Renderer2D::DrawRotatedQuad(pillar.top_position, pillar.top_scale, glm::radians(180.0f),
                                                   triangle_texture, 1.0f, color);
        chernoengine2::Renderer2D::DrawQuad(pillar.bottom_position, pillar.bottom_scale, triangle_texture, 1.0f, color);
    }

    player_.OnRender();
}


const Player& Level::GetPlayer() const {
    return player_;
}

void Level::CreatePillar(int index, float offset) {
    Pillar& pillar = pillars_[index];
    pillar.top_position.x = offset;
    pillar.bottom_position.x = offset;
    pillar.top_position.z = index * 0.1f - 0.5f;
    pillar.bottom_position.z = index * 0.1f - 0.5f + 0.05f;

    float center = Random::Get() * 35.0f - 17.5f;
    float gap = 2.0f + Random::Get() * 5.0f;

    pillar.top_position.y = 10.0f - ((10.0f - center) * 0.25f); /* + gap * 0.15f; */
    pillar.bottom_position.y = -10.0f - ((-10.0f - center) * 0.25f); /* - gap * 0.15f; */
}

bool Level::CollisionTest() {
    if (glm::abs(player_.GetPosition().y) > 8.5f)
        return true;

    glm::vec4 player_vertices[] = {
            {-0.5f, -0.5f, 0.0f, 1.0f},
            {0.5f,  -0.5f, 0.0f, 1.0f},
            {0.5f,  0.5f,  0.0f, 1.0f},
            {-0.5f, 0.5f,  0.0f, 1.0f}
    };

    const auto& player_pos = player_.GetPosition();
    glm::vec4 player_transformed_vertices[4];
    for (int i = 0; i < 4; i++) {
        player_transformed_vertices[i] = glm::translate(glm::mat4(1.0f), {player_pos.x, player_pos.y, 0.0f})
                                         * glm::rotate(glm::mat4(1.0f), glm::radians(player_.GetRotation()),
                                                       {0.0f, 0.0f, 1.0f})
                                         * glm::scale(glm::mat4(1.0f), {1.0f, 1.3f, 1.0f})
                                         * player_vertices[i];
    }

    // to match triangle.png (each corner is 10% from the texture edge)
    glm::vec4 pillar_vertices[] = {
            {-0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
            {0.5f - 0.1f,  -0.5f + 0.1f, 0.0f, 1.0f},
            {0.0f + 0.0f,  0.5f - 0.1f,  0.0f, 1.0f}
    };

    for (Pillar& p: pillars_) {
        glm::vec2 tri[3];

        // top pillar
        for (int i = 0; i < 3; i++) {
            tri[i] = glm::translate(glm::mat4(1.0f), {p.top_position.x, p.top_position.y, 0.0f})
                     * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), {0.0f, 0.0f, 1.0f})
                     * glm::scale(glm::mat4(1.0f), {p.top_scale.x, p.top_scale.y, 1.0f})
                     * pillar_vertices[i];
        }

        for (auto& vert: player_transformed_vertices) {
            if (PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2]))
                return true;
        }

        // bottom pillar
        for (int i = 0; i < 3; i++) {
            tri[i] = glm::translate(glm::mat4(1.0f), {p.bottom_position.x, p.bottom_position.y, 0.0f})
                     * glm::scale(glm::mat4(1.0f), {p.bottom_scale.x, p.bottom_scale.y, 1.0f})
                     * pillar_vertices[i];
        }

        for (auto& vert: player_transformed_vertices) {
            if (PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2]))
                return true;
        }
    }

    return false;
}

void Level::GameOver() {
    player_.Reset();

    pillar_target_ = 30.0f;
    pillar_index_ = 0;
}


} // firstgame