#include "spaceship.h"
#include "globals.h"
#include <cmath>

// Mathematical constants
constexpr float DEG120 = 2.0943951f; // 120° in radians
constexpr float DEG240 = 4.1887902f; // 240° in radians

/**
 * Constructor for Spaceship class
 * Initializes spaceship properties including triangle size, lives, and shield state
 * Calls Reset() to set initial position and movement values
 */
Spaceship::Spaceship() :
    triangleSize(15.0f),
    lives(STARTING_LIVES),
    shieldActive(false),
    shieldTimer(0.0f),
    shieldAnimationTimer(0.0f)
{
    Reset();
}

/**
 * Resets the spaceship to its initial state
 * Sets position to screen center, clears velocity and rotation
 * Preserves invulnerability state when called from LoseLife()
 * Deactivates shield and resets associated timers
 */
void Spaceship::Reset() {
    position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    velocity = { 0.0f, 0.0f };
    rotation = 0.0f;
    isThrusting = false;
    thrustCooldown = 0.0f;
    shieldActive = false;
    shieldTimer = 0.0f;
    shieldAnimationTimer = 0.0f;
    UpdateTriangleGeometry();
}

/**
 * Updates the spaceship's state each frame
 * Handles movement, screen wrapping, velocity dampening, and status timers
 * Updates invulnerability, thrust cooldown, and shield systems
 * Recalculates triangle geometry for rendering and collision
 * @param deltaTime Time elapsed since last frame in seconds
 */
void Spaceship::Update(float deltaTime) {
    // Movement
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    // Screen boundaries with wrapping
    position.x = fmodf(position.x + SCREEN_WIDTH, SCREEN_WIDTH);
    position.y = fmodf(position.y + SCREEN_HEIGHT, SCREEN_HEIGHT);

    // Friction/dampening
    velocity.x *= 0.99f;
    velocity.y *= 0.99f;

    // Status updates
    if (invulnerable) {
        invulnerabilityTimer -= deltaTime;
        if (invulnerabilityTimer <= 0.0f) invulnerable = false;
    }

    if (thrustCooldown > 0.0f) {
        thrustCooldown -= deltaTime;
    }

    // Shield update
    if (shieldActive) {
        shieldTimer -= deltaTime;
        shieldAnimationTimer += deltaTime;

        if (shieldTimer <= 0.0f) {
            shieldActive = false;
            shieldTimer = 0.0f;
        }
    }

    UpdateTriangleGeometry();
}

/**
 * Updates the triangle geometry based on current position and rotation
 * Calculates the three vertices of the spaceship triangle
 * Uses rotation matrix math for proper orientation
 * Called whenever position or rotation changes
 */
void Spaceship::UpdateTriangleGeometry() {
    const float rad = rotation * DEG2RAD;
    const float cosRot = cosf(rad);
    const float sinRot = sinf(rad);

    // Tip (forward point)
    trianglePoints[0] = {
        position.x + cosRot * triangleSize,
        position.y + sinRot * triangleSize
    };

    // Left point
    trianglePoints[1] = {
        position.x + (cosRot * cosf(DEG120) - sinRot * sinf(DEG120)) * triangleSize,
        position.y + (sinRot * cosf(DEG120) + cosRot * sinf(DEG120)) * triangleSize
    };

    // Right point
    trianglePoints[2] = {
        position.x + (cosRot * cosf(DEG240) - sinRot * sinf(DEG240)) * triangleSize,
        position.y + (sinRot * cosf(DEG240) + cosRot * sinf(DEG240)) * triangleSize
    };
}

/**
 * Renders the spaceship with shield effects and thrust particles
 * Handles invulnerability blinking, shield visualization, and thrust effects
 * Draws shield first (behind spaceship), then spaceship with optional blinking
 * Creates particle effects for thrust when active
 */
void Spaceship::Draw() const {
    // Draw shield (in front of spaceship) - without flickering
    if (shieldActive) {
        float shieldRadius = triangleSize + 8.0f;

        // Constant transparency - no pulsing
        Color shieldColor = BLUE;
        shieldColor.a = 100;

        // Outer shield ring
        DrawCircleV(position, shieldRadius, shieldColor);

        // Inner ring for depth
        Color innerColor = BLUE;
        innerColor.a = 50;
        DrawCircleV(position, shieldRadius - 2.0f, innerColor);

        // Shield border
        Color borderColor = BLUE;
        borderColor.a = 180;
        DrawCircleLines((int)position.x, (int)position.y, (int)shieldRadius, borderColor);

        // Additional energy effects (rotating particles)
        for (int i = 0; i < 6; i++) {
            float angle = (shieldAnimationTimer * 1.5f + (float)i * 60.0f) * DEG2RAD;
            Vector2 particlePos = {
                position.x + cosf(angle) * (shieldRadius - 1.0f),
                position.y + sinf(angle) * (shieldRadius - 1.0f)
            };

            Color particleColor = WHITE;
            particleColor.a = 120;
            DrawCircleV(particlePos, 1.0f, particleColor);
        }
    }

    // Spaceship visibility during invulnerability (blinking)
    bool shouldDrawShip = true;
    if (invulnerable) {
        // Fast blinking during invulnerability
        float blinkRate = 8.0f; // Blink 8 times per second
        shouldDrawShip = ((int)(invulnerabilityTimer * blinkRate) % 2) == 0;
    }

    if (!shouldDrawShip) return; // Don't draw spaceship when blinking

    // Draw spaceship body
    for (int i = 0; i < 3; i++) {
        int next = (i + 1) % 3;
        Vector2 p1 = {
            trianglePoints[i].x + (float)(GetRandomValue(0, 1)) - 0.5f,
            trianglePoints[i].y + (float)(GetRandomValue(0, 1)) - 0.5f
        };
        Vector2 p2 = {
            trianglePoints[next].x + (float)(GetRandomValue(0, 1)) - 0.5f,
            trianglePoints[next].y + (float)(GetRandomValue(0, 1)) - 0.5f
        };
        DrawLineV(p1, p2, BLACK);
    }

    // Thrust effect with particles
    if (isThrusting) {
        // Center of the rear edge of the spaceship
        Vector2 thrustBase = {
            (trianglePoints[1].x + trianglePoints[2].x) * 0.5f,
            (trianglePoints[1].y + trianglePoints[2].y) * 0.5f
        };

        // Direction vector (backward away from spaceship)
        Vector2 thrustDirection = {
            position.x - trianglePoints[0].x,
            position.y - trianglePoints[0].y
        };

        // Normalize
        float length = sqrtf(thrustDirection.x * thrustDirection.x + thrustDirection.y * thrustDirection.y);
        if (length > 0) {
            thrustDirection.x /= length;
            thrustDirection.y /= length;
        }

        // Generate particles
        for (int i = 0; i < GetRandomValue(4, 6); i++) {
            Vector2 particlePos = {
                thrustBase.x + (float)GetRandomValue(-5, 5) + thrustDirection.x * (float)GetRandomValue(10, 25),
                thrustBase.y + (float)GetRandomValue(-5, 5) + thrustDirection.y * (float)GetRandomValue(10, 25)
            };

            float size = (float)GetRandomValue(2, 6);
            Color particleColor;
            int colorRand = GetRandomValue(0, 100);
            if (colorRand < 60) particleColor = ORANGE;
            else if (colorRand < 90) particleColor = RED;
            else particleColor = YELLOW;

            DrawCircleV(particlePos, size + 1, Fade(particleColor, 0.4f));
            DrawCircleV(particlePos, size, particleColor);
        }
    }
}

/**
 * Applies thrust force to the spaceship in the direction it's facing
 * Calculates thrust direction from triangle geometry and applies acceleration
 * Enforces maximum speed limit to prevent uncontrolled acceleration
 * Only applies thrust when isThrusting flag is active
 * @param deltaTime Time elapsed since last frame for smooth acceleration
 */
void Spaceship::ApplyThrust(float deltaTime) {
    if (!isThrusting) return;

    Vector2 thrustDirection = {
        trianglePoints[0].x - position.x,
        trianglePoints[0].y - position.y
    };

    // Normalize
    float length = sqrtf(thrustDirection.x * thrustDirection.x + thrustDirection.y * thrustDirection.y);
    if (length > 0.0f) {
        thrustDirection.x /= length;
        thrustDirection.y /= length;

        velocity.x += thrustDirection.x * SPACESHIP_ACCELERATION * deltaTime;
        velocity.y += thrustDirection.y * SPACESHIP_ACCELERATION * deltaTime;

        // Speed limiting
        float speed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
        if (speed > MAX_SPACESHIP_SPEED) {
            velocity.x = (velocity.x / speed) * MAX_SPACESHIP_SPEED;
            velocity.y = (velocity.y / speed) * MAX_SPACESHIP_SPEED;
        }
    }
}

/**
 * Activates thrust mode for the spaceship
 * Sets the thrusting flag to enable thrust application and visual effects
 */
void Spaceship::StartThrust() {
    isThrusting = true;
}

/**
 * Deactivates thrust mode for the spaceship
 * Clears the thrusting flag to stop thrust application and visual effects
 */
void Spaceship::StopThrust() {
    isThrusting = false;
}

/**
 * Rotates the spaceship by the specified direction and speed
 * Updates rotation angle based on direction, rotation speed, and delta time
 * @param direction Rotation direction (-1 for left, +1 for right)
 * @param deltaTime Time elapsed since last frame for smooth rotation
 */
void Spaceship::Rotate(float direction, float deltaTime) {
    rotation += direction * SPACESHIP_ROTATION_SPEED * deltaTime;
}

/**
 * Activates the protective shield around the spaceship
 * Sets shield duration to 8 seconds and resets animation timer
 * Provides temporary protection from collisions
 */
void Spaceship::ActivateShield() {
    shieldActive = true;
    shieldTimer = 8.0f; // 8 seconds shield duration
    shieldAnimationTimer = 0.0f;
}

/**
 * Deactivates the protective shield
 * Immediately removes shield protection and resets timer
 */
void Spaceship::DeactivateShield() {
    shieldActive = false;
    shieldTimer = 0.0f;
}

/**
 * Checks if the shield is currently active
 * @return True if shield is providing protection, false otherwise
 */
bool Spaceship::IsShieldActive() const {
    return shieldActive;
}

/**
 * Gets the remaining time for shield protection
 * @return Float value representing seconds remaining for shield
 */
float Spaceship::GetShieldTimeRemaining() const {
    return shieldTimer;
}

// Getter methods for spaceship state access
/**
 * Gets the current position of the spaceship
 * @return Vector2 containing x,y coordinates of spaceship center
 */
Vector2 Spaceship::GetPosition() const { 
    return position;
}

/**
 * Gets the current rotation angle of the spaceship
 * @return Float value representing rotation in degrees
 */
float Spaceship::GetRotation() const { 
    return rotation; 
}

/**
 * Gets the current velocity vector of the spaceship
 * @return Vector2 containing x,y velocity components
 */
Vector2 Spaceship::GetVelocity() const { 
    return velocity;
}

/**
 * Sets the invulnerability timer to a fixed duration
 * @param value Unused parameter, timer is always set to 3.0 seconds
 */
void Spaceship::setInvulnerableTimer(float value) {
    invulnerabilityTimer = 3.0f; 
}

/**
 * Sets the invulnerability status of the spaceship
 * @param value Boolean flag for invulnerability state
 */
void Spaceship::setIsInvulnerable(bool value) { 
    invulnerable = value;
}

/**
 * Checks if the spaceship is currently invulnerable
 * @return True if spaceship cannot take damage, false otherwise
 */
bool Spaceship::IsInvulnerable() const {
    return invulnerable; 
}

/**
 * Gets the remaining invulnerability time
 * @return Float value representing seconds remaining for invulnerability
 */
float Spaceship::GetInvulnerabilityTime() const {
    return invulnerabilityTimer;
}

/**
 * Checks if the spaceship is currently thrusting
 * @return True if thrust is being applied, false otherwise
 */
bool Spaceship::IsThrusting() const { 
    return isThrusting;
}

/**
 * Gets the current number of lives remaining
 * @return Integer count of remaining lives
 */
int Spaceship::GetLives() const { 
    return lives;
}

/**
 * Adds one extra life to the spaceship
 * Increments the lives counter by one
 */
void Spaceship::AddLife() { 
    lives++; 
}

/**
 * Gets the collision bounding rectangle for the spaceship
 * Creates a square bounding box centered on the spaceship position
 * @return Rectangle representing collision boundaries
 */
Rectangle Spaceship::GetBounds() const {
    return { position.x - triangleSize, position.y - triangleSize,
             triangleSize * 2.0f, triangleSize * 2.0f };
}

/**
 * Handles losing a life and respawning with temporary invulnerability
 * Decrements life count, activates invulnerability, deactivates shield
 * Resets position and movement while preserving invulnerability status
 * Critical: Sets invulnerability BEFORE calling Reset() to prevent override
 */
void Spaceship::LoseLife() {
    lives--;
    // FIRST set invulnerability
    invulnerable = true;
    invulnerabilityTimer = 3.0f; // 3 seconds invulnerability after respawn

    // Deactivate shield when life is lost
    DeactivateShield();

    // THEN call Reset() - but without overriding invulnerability
    Reset();
}