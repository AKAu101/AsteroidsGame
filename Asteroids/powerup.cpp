#include "powerup.h"
#include "globals.h"
#include <cmath>

PowerUp::PowerUp() {
    active = false;
    animationTimer = 0.0f;
    position = { 0.0f, 0.0f };  // Initialize position
    type = EXTRA_LIFE;        // Initialize type
    lifetime = 0.0f;          // Initialize lifetime
}

void PowerUp::Spawn(Vector2 pos, PowerUpType powerType) {
    position = pos;
    type = powerType;
    lifetime = POWERUP_LIFETIME;
    active = true;
    animationTimer = 0.0f;
}

void PowerUp::Update(float deltaTime) {
    if (!active) return;

    lifetime -= deltaTime;
    animationTimer += deltaTime;

    if (lifetime <= 0) {
        active = false;
    }
}

void PowerUp::Draw() const {  // Made const
    if (!active) return;

    Color color = WHITE;
    const char* text = "";

    switch (type) {
    case EXTRA_LIFE:
        color = GREEN;
        text = "LIFE";
        break;
    case RAPID_FIRE:
        color = RED;
        text = "RAPID";
        break;
    case SHIELD:
        color = BLUE;
        text = "SHIELD";
        break;
    }

    // Pulsing effect
    float alpha = 0.5f + 0.5f * sin(animationTimer * 5);
    color.a = static_cast<unsigned char>(255 * alpha);

    DrawRectangle(static_cast<int>(position.x - 25), static_cast<int>(position.y - 10), 50, 20, color);
    DrawText(text, static_cast<int>(position.x - MeasureText(text, 10) / 2), static_cast<int>(position.y - 5), 10, BLACK);
}

Rectangle PowerUp::GetBounds() const {
    return { position.x - 25, position.y - 10, 50, 20 };
}