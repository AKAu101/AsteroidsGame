#include "asteroid.h"
#include "globals.h"
#include <cmath>
#include <cstdlib>

// Color Palette for Asteroids
static const Color ASTEROID_COLORS[] = { RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE };
static const int NUM_ASTEROID_COLORS = sizeof(ASTEROID_COLORS) / sizeof(ASTEROID_COLORS[0]);

// Constructor
Asteroid::Asteroid() {
    active = false;
    position = { 0.0f, 0.0f };
    velocity = { 0.0f, 0.0f };
    size = LARGE;
    rotation = 0.0f;
    rotationSpeed = 0.0f;
    colorIndex = 0;
}

void Asteroid::Spawn(Vector2 pos, AsteroidSize asteroidSize) {
    position = pos;
    size = asteroidSize;
    active = true;

    // Progressive Geschwindigkeit basierend auf globalem Score
    float baseMinSpeed = ASTEROID_MIN_SPEED;
    float baseMaxSpeed = ASTEROID_MAX_SPEED;

    // Geschwindigkeits-Multiplikator basierend auf globaler Score-Variable
    float speedMultiplier = 1.0f + (g_currentScore / 5000.0f); // Alle 5000 Punkte 100% schneller
    speedMultiplier = SafeMin(speedMultiplier, 3.0f); // Maximum 3x so schnell

    float minSpeed = baseMinSpeed * speedMultiplier;
    float maxSpeed = baseMaxSpeed * speedMultiplier;

    // Set random movement mit progressiver Geschwindigkeit
    float speed = minSpeed + (float)(rand() % (int)(maxSpeed - minSpeed + 1));
    float angle = (float)(rand() % 360) * WINKEL2GRAD;

    velocity.x = cosf(angle) * speed;
    velocity.y = sinf(angle) * speed;

    // Set random rotation (auch schneller)
    rotation = 0.0f;
    rotationSpeed = ((float)(rand() % 100 - 50) / 10.0f) * speedMultiplier;

    // Assign random color
    colorIndex = rand() % NUM_ASTEROID_COLORS;
}

void Asteroid::Update(float deltaTime) {
    if (!active) return;
    // Update position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    // Update rotation
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
    int segments = 12;

    // 1. Gefüllten Körper zeichnen
    DrawPoly(position, segments, radius, rotation, ASTEROID_COLORS[colorIndex]);

    // 2. Umrandung zeichnen (direkt ohne Hilfsmethode)
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 360.0f / segments + rotation) * DEG2RAD;
        float angle2 = ((i + 1) * 360.0f / segments + rotation) * DEG2RAD;
        Vector2 p1 = { position.x + cosf(angle1) * radius, position.y + sinf(angle1) * radius };
        Vector2 p2 = { position.x + cosf(angle2) * radius, position.y + sinf(angle2) * radius };
        DrawLineEx(p1, p2, 2.0f, BLACK);
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
    case LARGE: return (float)LARGE_ASTEROID_SIZE;
    case MEDIUM: return (float)MEDIUM_ASTEROID_SIZE;
    case SMALL: return (float)SMALL_ASTEROID_SIZE;
    }
    return 0.0f;
}