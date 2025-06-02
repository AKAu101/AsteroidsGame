#include "projectile.h"
#include "globals.h"
#include <cmath>

/**
 * Default constructor for Projectile class
 * Initializes all member variables to default inactive state
 * Sets position, velocity, lifetime, and active status to safe defaults
 */
Projectile::Projectile() {
    position = { 0.0f, 0.0f };
    velocity = { 0.0f, 0.0f };
    lifetime = 0.0f;
    active = false;
}

/**
 * Fires a projectile from the specified position in the given direction
 * Calculates velocity based on rotation angle and projectile speed
 * Activates the projectile and sets its lifetime
 * @param startPos Starting position for the projectile
 * @param rotation Direction angle in degrees for projectile movement
 */
void Projectile::Fire(Vector2 startPos, float rotation) {
    position = startPos;
    float radians = rotation * WINKEL2GRAD;
    velocity.x = cosf(radians) * PROJECTILE_SPEED;
    velocity.y = sinf(radians) * PROJECTILE_SPEED;
    lifetime = PROJECTILE_LIFETIME;
    active = true;
}

/**
 * Updates the projectile's position and state each frame
 * Moves projectile based on velocity, decrements lifetime
 * Handles screen wrapping - projectiles reappear on opposite side
 * Deactivates projectile when lifetime expires
 * Only processes updates if the projectile is active
 * @param deltaTime Time elapsed since last frame in seconds
 */
void Projectile::Update(float deltaTime) {
    if (!active) return;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    lifetime -= deltaTime;

    // Screen wrapping - projectiles wrap around screen edges
    if (position.x < 0) position.x = SCREEN_WIDTH;
    if (position.x > SCREEN_WIDTH) position.x = 0;
    if (position.y < 0) position.y = SCREEN_HEIGHT;
    if (position.y > SCREEN_HEIGHT) position.y = 0;

    if (lifetime <= 0) {
        active = false;
    }
}

/**
 * Renders the projectile as a black rectangle oriented in movement direction
 * Uses rotated rectangle drawing for proper orientation
 * Creates a elongated shape (4x12 pixels) for better visibility
 * Aligns the projectile vertically relative to its movement direction
 * Only renders if the projectile is active
 */
void Projectile::Draw() const {
    if (!active) return;

    // Black rectangle
    Rectangle rect = {
        position.x - 2.0f,  // Centered horizontally
        position.y - 6.0f,  // 6 pixels upward offset
        4.0f,               // Width
        12.0f               // Height
    };

    // Orientation aligned with movement direction
    float angle = atan2f(velocity.y, velocity.x) * RAD2DEG + 90.0f; // +90° for vertical alignment

    DrawRectanglePro(
        rect,
        Vector2{ 2.0f, 6.0f },  // Pivot point at center
        angle,
        BLACK
    );
}

/**
 * Returns the bounding rectangle for collision detection
 * Creates a small square collision box centered on the projectile's position
 * Uses 4x4 pixel dimensions for precise collision detection
 * @return Rectangle representing the projectile's collision bounds
 */
Rectangle Projectile::GetBounds() const {
    return { position.x - 2, position.y - 2, 4, 4 };
}