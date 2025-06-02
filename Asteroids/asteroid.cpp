#include "asteroid.h"
#include "globals.h"
#include <cmath>
#include <cstdlib>

// Color palette for asteroids
static const Color ASTEROID_COLORS[] = { RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE };
static const int NUM_ASTEROID_COLORS = sizeof(ASTEROID_COLORS) / sizeof(ASTEROID_COLORS[0]);

/**
 * Default constructor for Asteroid class
 * Initializes all member variables to default inactive state
 * Sets position, velocity, and rotation values to zero
 */
Asteroid::Asteroid() {
    active = false;
    position = { 0.0f, 0.0f };
    velocity = { 0.0f, 0.0f };
    size = LARGE;
    rotation = 0.0f;
    rotationSpeed = 0.0f;
    colorIndex = 0;
}

/**
 * Spawns an asteroid at the specified position with given size
 * Calculates progressive speed based on global score for increased difficulty
 * Sets random movement direction, rotation speed, and color
 * @param pos Starting position for the asteroid
 * @param asteroidSize Size category (LARGE, MEDIUM, or SMALL)
 */
void Asteroid::Spawn(Vector2 pos, AsteroidSize asteroidSize) {
    position = pos;
    size = asteroidSize;
    active = true;

    // Progressive speed based on global score
    float baseMinSpeed = ASTEROID_MIN_SPEED;
    float baseMaxSpeed = ASTEROID_MAX_SPEED;

    // Speed multiplier based on global score variable
    float speedMultiplier = 1.0f + (g_currentScore / 5000.0f); // 100% faster every 5000 points
    speedMultiplier = SafeMin(speedMultiplier, 3.0f); // Maximum 3x as fast

    float minSpeed = baseMinSpeed * speedMultiplier;
    float maxSpeed = baseMaxSpeed * speedMultiplier;

    // Set random movement with progressive speed
    float speed = minSpeed + (float)(rand() % (int)(maxSpeed - minSpeed + 1));
    float angle = (float)(rand() % 360) * WINKEL2GRAD;
    velocity.x = cosf(angle) * speed;
    velocity.y = sinf(angle) * speed;

    // Set random rotation (also faster with progression)
    rotation = 0.0f;
    rotationSpeed = ((float)(rand() % 100 - 50) / 10.0f) * speedMultiplier;

    // Assign random color
    colorIndex = rand() % NUM_ASTEROID_COLORS;
}

/**
 * Updates the asteroid's position and rotation each frame
 * Handles movement based on velocity and screen wrapping at boundaries
 * Only processes updates if the asteroid is active
 */
void Asteroid::Update(float deltaTime) {
    if (!active) return;

    // Update position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    // Update rotation
    rotation += rotationSpeed * deltaTime;

    // Screen wrapping - asteroids reappear on opposite side when leaving screen
    float radius = GetRadius();
    if (position.x < -radius) position.x = SCREEN_WIDTH + radius;
    if (position.x > SCREEN_WIDTH + radius) position.x = -radius;
    if (position.y < -radius) position.y = SCREEN_HEIGHT + radius;
    if (position.y > SCREEN_HEIGHT + radius) position.y = -radius;
}

/**
 * Renders the asteroid with filled body and black outline
 * Uses polygon drawing for the main body with the assigned color
 * Draws individual line segments for the border outline
 * Only renders if the asteroid is active
 */
void Asteroid::Draw() const {
    if (!active) return;

    float radius = GetRadius();
    int segments = 12;

    // 1. Draw filled body
    DrawPoly(position, segments, radius, rotation, ASTEROID_COLORS[colorIndex]);

    // 2. Draw outline (directly without helper method)
    for (int i = 0; i < segments; i++) {
        float angle1 = (i * 360.0f / segments + rotation) * DEG2RAD;
        float angle2 = ((i + 1) * 360.0f / segments + rotation) * DEG2RAD;

        Vector2 p1 = { position.x + cosf(angle1) * radius, position.y + sinf(angle1) * radius };
        Vector2 p2 = { position.x + cosf(angle2) * radius, position.y + sinf(angle2) * radius };

        DrawLineEx(p1, p2, 2.0f, BLACK);
    }
}

/**
 * Returns the bounding rectangle for collision detection
 * Creates a square bounding box centered on the asteroid's position
 * @return Rectangle representing the asteroid's collision bounds
 */
Rectangle Asteroid::GetBounds() const {
    float radius = GetRadius();
    return { position.x - radius, position.y - radius, radius * 2, radius * 2 };
}

/**
 * Returns the point value awarded for destroying this asteroid
 * Larger asteroids give fewer points, smaller ones give more points
 * @return Integer point value based on asteroid size
 */
int Asteroid::GetPoints() const {
    switch (size) {
    case LARGE: return LARGE_ASTEROID_POINTS;
    case MEDIUM: return MEDIUM_ASTEROID_POINTS;
    case SMALL: return SMALL_ASTEROID_POINTS;
    }
    return 0;
}

/**
 * Returns the radius of the asteroid based on its size category
 * Used for collision detection, rendering, and screen wrapping calculations
 * @return Float radius value corresponding to the asteroid's size
 */
float Asteroid::GetRadius() const {
    switch (size) {
    case LARGE: return (float)LARGE_ASTEROID_SIZE;
    case MEDIUM: return (float)MEDIUM_ASTEROID_SIZE;
    case SMALL: return (float)SMALL_ASTEROID_SIZE;
    }
    return 0.0f;
}