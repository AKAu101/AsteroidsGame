#include "powerup.h"
#include "globals.h"
#include <cmath>

/**
 * Default constructor for PowerUp class
 * Initializes all member variables to default inactive state
 * Sets position, type, and timers to safe default values
 */
PowerUp::PowerUp() {
    active = false;
    animationTimer = 0.0f;
    position = { 0.0f, 0.0f };  // Initialize position
    type = EXTRA_LIFE;          // Initialize type
    lifetime = 0.0f;            // Initialize lifetime
}

/**
 * Spawns a power-up at the specified position with given type
 * Activates the power-up and sets its lifetime and animation timer
 * @param pos 2D vector position where the power-up should appear
 * @param powerType Type of power-up (EXTRA_LIFE, RAPID_FIRE, or SHIELD)
 */
void PowerUp::Spawn(Vector2 pos, PowerUpType powerType) {
    position = pos;
    type = powerType;
    lifetime = POWERUP_LIFETIME;
    active = true;
    animationTimer = 0.0f;
}

/**
 * Updates the power-up's state each frame
 * Decrements lifetime and updates animation timer
 * Deactivates power-up when lifetime expires
 * Only processes updates if the power-up is active
 * @param deltaTime Time elapsed since last frame in seconds
 */
void PowerUp::Update(float deltaTime) {
    if (!active) return;

    lifetime -= deltaTime;
    animationTimer += deltaTime;

    if (lifetime <= 0) {
        active = false;
    }
}

/**
 * Renders the power-up with visual effects and text label
 * Uses different colors for each power-up type with pulsing animation
 * Displays type-specific text centered on the power-up
 * Creates alpha pulsing effect using sine wave for visibility
 * Only renders if the power-up is active
 */
void PowerUp::Draw() const {
    if (!active) return;

    Color color = WHITE;
    const char* text = "";

    // Set color and text based on power-up type
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

    // Create pulsing effect using sine wave
    float alpha = 0.5f + 0.5f * sin(animationTimer * 5);
    color.a = static_cast<unsigned char>(255 * alpha);

    // Draw power-up background rectangle
    DrawRectangle(static_cast<int>(position.x - 25), static_cast<int>(position.y - 10), 50, 20, color);

    // Draw centered text label
    DrawText(text, static_cast<int>(position.x - MeasureText(text, 10) / 2), static_cast<int>(position.y - 5), 10, BLACK);
}

/**
 * Returns the bounding rectangle for collision detection
 * Creates a rectangular collision box centered on the power-up's position
 * Rectangle dimensions match the visual representation (50x20 pixels)
 * @return Rectangle representing the power-up's collision bounds
 */
Rectangle PowerUp::GetBounds() const {
    return { position.x - 25, position.y - 10, 50, 20 };
}