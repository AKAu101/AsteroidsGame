#include "asteroid.h"
#include "globals.h"
#include <cmath>
#include <cstdlib>

Asteroid::Asteroid() {
    active = false;
    position = { 0.0f, 0.0f };
    velocity = { 0.0f, 0.0f };
    size = LARGE;
    rotation = 0.0f;
    rotationSpeed = 0.0f;
    colorIndex = 0; // Initialize color index
}

void Asteroid::Spawn(Vector2 pos, AsteroidSize asteroidSize) {
    position = pos;
    size = asteroidSize;

    float speed = ASTEROID_MIN_SPEED + (rand() % static_cast<int>(ASTEROID_MAX_SPEED - ASTEROID_MIN_SPEED));
    float angle = (rand() % 360) * WINKEL2GRAD;

    velocity.x = cos(angle) * speed;
    velocity.y = sin(angle) * speed;

    rotation = 0.0f;
    rotationSpeed = (rand() % 100 - 50) / 10.0f;

    // Assign random color index
    colorIndex = rand() % 6; // 6 different colors

    active = true;
}

void Asteroid::Update(float deltaTime) {
    if (!active) return;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    rotation += rotationSpeed * deltaTime;

    // Screen wrapping
    float radius = GetRadius();
    if (position.x < -radius) position.x = SCREEN_WIDTH + radius;
    if (position.x > SCREEN_WIDTH + radius) position.x = -radius;
    if (position.y < -radius) position.y = SCREEN_HEIGHT + radius;
    if (position.y > SCREEN_HEIGHT + radius) position.y = -radius;
}

void Asteroid::Draw() const {
    if (!active) return;

    float radius = GetRadius();
    int segments = 8;

    // Color array for different asteroid colors - made darker for better contrast
    Color colors[6] = {
        {200, 50, 50, 255},   // Dark Red
        {50, 200, 50, 255},   // Dark Green  
        {50, 50, 200, 255},   // Dark Blue
        {200, 200, 50, 255},  // Dark Yellow
        {200, 50, 200, 255},  // Dark Magenta
        {50, 200, 200, 255}   // Dark Cyan
    };

    Color asteroidColor = colors[colorIndex % 6];

    // Draw filled asteroid with gradient effect
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 360.0f / segments + rotation) * WINKEL2GRAD;
        float angle2 = ((i + 1) * 360.0f / segments + rotation) * WINKEL2GRAD;

        Vector2 p1 = {
            position.x + cos(angle1) * radius,
            position.y + sin(angle1) * radius
        };
        Vector2 p2 = {
            position.x + cos(angle2) * radius,
            position.y + sin(angle2) * radius
        };

        // Draw triangle from center to create filled effect
        DrawTriangle(position, p1, p2, Color{
            static_cast<unsigned char>(asteroidColor.r * 0.6f),
            static_cast<unsigned char>(asteroidColor.g * 0.6f),
            static_cast<unsigned char>(asteroidColor.b * 0.6f),
            200
            });
    }

    // Draw simple colored outline for visibility
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 360.0f / segments + rotation) * WINKEL2GRAD;
        float angle2 = ((i + 1) * 360.0f / segments + rotation) * WINKEL2GRAD;

        Vector2 p1 = {
            position.x + cos(angle1) * radius,
            position.y + sin(angle1) * radius
        };
        Vector2 p2 = {
            position.x + cos(angle2) * radius,
            position.y + sin(angle2) * radius
        };

        // Draw outline with brighter asteroid color
        DrawLineV(p1, p2, asteroidColor);
    }

    // Add some surface details
    if (size == LARGE) {
        // Draw some craters/spots
        DrawCircleV({ position.x + radius * 0.3f, position.y + radius * 0.2f }, 3,
            Color{
                static_cast<unsigned char>(asteroidColor.r * 0.4f),
                static_cast<unsigned char>(asteroidColor.g * 0.4f),
                static_cast<unsigned char>(asteroidColor.b * 0.4f),
                255
            });
        DrawCircleV({ position.x - radius * 0.2f, position.y - radius * 0.4f }, 2,
            Color{
                static_cast<unsigned char>(asteroidColor.r * 0.4f),
                static_cast<unsigned char>(asteroidColor.g * 0.4f),
                static_cast<unsigned char>(asteroidColor.b * 0.4f),
                255
            });
    }
}

Rectangle Asteroid::GetBounds() const {
    float radius = GetRadius();
    return { position.x - radius, position.y - radius, radius * 2, radius * 2 };
}

int Asteroid::GetPoints() const {
    switch (size) {
    case LARGE: return LARGE_ASTEROID_POINTS;
    case MEDIUM: return MEDIUM_ASTEROID_POINTS;
    case SMALL: return SMALL_ASTEROID_POINTS;
    }
    return 0;
}

float Asteroid::GetRadius() const {
    switch (size) {
    case LARGE: return static_cast<float>(LARGE_ASTEROID_SIZE);
    case MEDIUM: return static_cast<float>(MEDIUM_ASTEROID_SIZE);
    case SMALL: return static_cast<float>(SMALL_ASTEROID_SIZE);
    }
    return 0.0f;
}