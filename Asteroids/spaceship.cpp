#include "spaceship.h"
#include "globals.h"
#include <cmath>

// Mathematische Konstanten
constexpr float DEG120 = 2.0943951f; // 120° in Rad
constexpr float DEG240 = 4.1887902f; // 240° in Rad

Spaceship::Spaceship() : 
    triangleSize(15.0f),
    lives(STARTING_LIVES) 
{
    Reset();
}

void Spaceship::Reset() {
    position = { SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f };
    velocity = { 0, 0 };
    rotation = 0;
    isThrusting = false;
    invulnerable = false;
    invulnerabilityTimer = 0.0f;
    thrustCooldown = 0;
    UpdateTriangleGeometry();
}

void Spaceship::Update(float deltaTime) {
    // Bewegung
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    // Bildschirmbegrenzung
    position.x = fmod(position.x + SCREEN_WIDTH, SCREEN_WIDTH);
    position.y = fmod(position.y + SCREEN_HEIGHT, SCREEN_HEIGHT);

    // Reibung
    velocity.x *= 0.99f;
    velocity.y *= 0.99f;

    // Statusupdates
    if (invulnerable) {
        invulnerabilityTimer -= deltaTime;
        if (invulnerabilityTimer <= 0) invulnerable = false;
    }
    
    if (thrustCooldown > 0) {
        thrustCooldown -= deltaTime;
    }

    UpdateTriangleGeometry();
}

void Spaceship::UpdateTriangleGeometry() {
    const float rad = rotation * DEG2RAD;
    const float cosRot = cos(rad);
    const float sinRot = sin(rad);
    
    // Spitze (vorwärts)
    trianglePoints[0] = {
        position.x + cosRot * triangleSize,
        position.y + sinRot * triangleSize
    };
    
    // Linker Punkt
    trianglePoints[1] = {
        position.x + (cosRot * cos(DEG120) - sinRot * sin(DEG120)) * triangleSize,
        position.y + (sinRot * cos(DEG120) + cosRot * sin(DEG120)) * triangleSize
    };
    
    // Rechter Punkt
    trianglePoints[2] = {
        position.x + (cosRot * cos(DEG240) - sinRot * sin(DEG240)) * triangleSize,
        position.y + (sinRot * cos(DEG240) + cosRot * sin(DEG240)) * triangleSize
    };
}

void Spaceship::Draw() const {
    if (invulnerable && ((int)(invulnerabilityTimer * 10) % 2)) return;
    
    // Raumschiffkörper
    DrawLineV(trianglePoints[1], trianglePoints[0], BLACK); // Links zur Spitze
    DrawLineV(trianglePoints[2], trianglePoints[0], BLACK); // Rechts zur Spitze
    
    // Schubeffekt
    if (isThrusting) {
        Vector2 thrustBase = {
            (trianglePoints[1].x + trianglePoints[2].x) * 0.5f,
            (trianglePoints[1].y + trianglePoints[2].y) * 0.5f
        };
        DrawLineV(trianglePoints[1], thrustBase, ORANGE);
        DrawLineV(trianglePoints[2], thrustBase, ORANGE);
    }
}

void Spaceship::ApplyThrust(float deltaTime) {
    if (!isThrusting) return;
    
    Vector2 thrustDirection = {
        trianglePoints[0].x - position.x,
        trianglePoints[0].y - position.y
    };
    
    // Normalisieren
    float length = sqrt(thrustDirection.x*thrustDirection.x + thrustDirection.y*thrustDirection.y);
    if (length > 0) {
        thrustDirection.x /= length;
        thrustDirection.y /= length;
        
        velocity.x += thrustDirection.x * SPACESHIP_ACCELERATION * deltaTime;
        velocity.y += thrustDirection.y * SPACESHIP_ACCELERATION * deltaTime;
        
        // Geschwindigkeitsbegrenzung
        float speed = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
        if (speed > MAX_SPACESHIP_SPEED) {
            velocity.x = (velocity.x/speed) * MAX_SPACESHIP_SPEED;
            velocity.y = (velocity.y/speed) * MAX_SPACESHIP_SPEED;
        }
    }
}

// Restliche Methoden...
void Spaceship::StartThrust() { isThrusting = true; }
void Spaceship::StopThrust() { isThrusting = false; }
void Spaceship::Rotate(float direction, float deltaTime) {
    rotation += direction * SPACESHIP_ROTATION_SPEED * deltaTime;
}
Vector2 Spaceship::GetPosition() const { return position; }
float Spaceship::GetRotation() const { return rotation; }
Vector2 Spaceship::GetVelocity() const { return velocity; }
bool Spaceship::IsInvulnerable() const { return invulnerable; }
bool Spaceship::IsThrusting() const { return isThrusting; }
int Spaceship::GetLives() const { return lives; }
void Spaceship::AddLife() { lives++; }

Rectangle Spaceship::GetBounds() const {
    return { position.x - triangleSize, position.y - triangleSize,
             triangleSize * 2, triangleSize * 2 };
}

void Spaceship::LoseLife() {
    lives--;
    invulnerable = true;
    invulnerabilityTimer = 3.0f;
    Reset();
}