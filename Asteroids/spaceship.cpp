#include "spaceship.h"
#include "globals.h"
#include "utils.h"
#include <cmath>

// Mathematische Konstanten
constexpr float DEG120 = 2.0943951f; // 120° in Rad
constexpr float DEG240 = 4.1887902f; // 240° in Rad

Spaceship::Spaceship() :
    triangleSize(15.0f),
    lives(STARTING_LIVES),
    shieldActive(false),
    shieldTimer(0.0f),
    shieldAnimationTimer(0.0f)
{
    Reset();
}

void Spaceship::Reset() {
    position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    velocity = { 0.0f, 0.0f };
    rotation = 0.0f;
    isThrusting = false;
    // WICHTIG: invulnerable und invulnerabilityTimer NICHT zurücksetzen!
    // Diese werden in LoseLife() bereits gesetzt
    thrustCooldown = 0.0f;
    shieldActive = false;
    shieldTimer = 0.0f;
    shieldAnimationTimer = 0.0f;
    UpdateTriangleGeometry();
}

void Spaceship::Update(float deltaTime) {
    // Bewegung
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    // Bildschirmbegrenzung
    position.x = fmodf(position.x + SCREEN_WIDTH, SCREEN_WIDTH);
    position.y = fmodf(position.y + SCREEN_HEIGHT, SCREEN_HEIGHT);

    // Reibung
    velocity.x *= 0.99f;
    velocity.y *= 0.99f;

    // Statusupdates
    if (invulnerable) {
        invulnerabilityTimer -= deltaTime;
        if (invulnerabilityTimer <= 0.0f) invulnerable = false;
    }

    if (thrustCooldown > 0.0f) {
        thrustCooldown -= deltaTime;
    }

    // Schild-Update
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

void Spaceship::UpdateTriangleGeometry() {
    const float rad = rotation * DEG2RAD;
    const float cosRot = cosf(rad);
    const float sinRot = sinf(rad);

    // Spitze (vorwärts)
    trianglePoints[0] = {
        position.x + cosRot * triangleSize,
        position.y + sinRot * triangleSize
    };

    // Linker Punkt
    trianglePoints[1] = {
        position.x + (cosRot * cosf(DEG120) - sinRot * sinf(DEG120)) * triangleSize,
        position.y + (sinRot * cosf(DEG120) + cosRot * sinf(DEG120)) * triangleSize
    };

    // Rechter Punkt
    trianglePoints[2] = {
        position.x + (cosRot * cosf(DEG240) - sinRot * sinf(DEG240)) * triangleSize,
        position.y + (sinRot * cosf(DEG240) + cosRot * sinf(DEG240)) * triangleSize
    };
}

void Spaceship::Draw() const {
    // Schild zeichnen (vor dem Raumschiff) - ohne Flackern
    if (shieldActive) {
        float shieldRadius = triangleSize + 8.0f; // Etwas größer als das Raumschiff

        // Konstante Transparenz - kein Pulsieren
        Color shieldColor = BLUE;
        shieldColor.a = 100; // Feste Transparenz

        // Äußerer Schild-Ring
        DrawCircleV(position, shieldRadius, shieldColor);

        // Innerer Ring für Tiefe
        Color innerColor = BLUE;
        innerColor.a = 50; // Feste Transparenz
        DrawCircleV(position, shieldRadius - 2.0f, innerColor);

        // Schild-Umrandung
        Color borderColor = BLUE;
        borderColor.a = 180; // Feste Transparenz
        DrawCircleLines((int)position.x, (int)position.y, (int)shieldRadius, borderColor);

        // Zusätzliche Energie-Effekte (rotierende Partikel)
        for (int i = 0; i < 6; i++) {
            float angle = (shieldAnimationTimer * 1.5f + (float)i * 60.0f) * DEG2RAD;
            Vector2 particlePos = {
                position.x + cosf(angle) * (shieldRadius - 1.0f),
                position.y + sinf(angle) * (shieldRadius - 1.0f)
            };

            Color particleColor = WHITE;
            particleColor.a = 120; // Feste Transparenz
            DrawCircleV(particlePos, 1.0f, particleColor);
        }
    }

    // Raumschiff-Sichtbarkeit bei Unverwundbarkeit (Blinken)
    bool shouldDrawShip = true;
    if (invulnerable) {
        // Schnelles Blinken während Unverwundbarkeit
        float blinkRate = 8.0f; // 8 mal pro Sekunde blinken
        shouldDrawShip = ((int)(invulnerabilityTimer * blinkRate) % 2) == 0;
    }

    if (!shouldDrawShip) return; // Raumschiff nicht zeichnen wenn es blinkt

    // Raumschiffkörper zeichnen
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

    // Schubeffekt mit Partikeln
    if (isThrusting) {
        // Mitte der hinteren Kante des Raumschiffs
        Vector2 thrustBase = {
            (trianglePoints[1].x + trianglePoints[2].x) * 0.5f,
            (trianglePoints[1].y + trianglePoints[2].y) * 0.5f
        };

        // Richtungsvektor (nach hinten weg vom Raumschiff)
        Vector2 thrustDirection = {
            position.x - trianglePoints[0].x,
            position.y - trianglePoints[0].y
        };

        // Normalisieren
        float length = sqrtf(thrustDirection.x * thrustDirection.x + thrustDirection.y * thrustDirection.y);
        if (length > 0) {
            thrustDirection.x /= length;
            thrustDirection.y /= length;
        }

        // Partikel erzeugen
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

void Spaceship::ApplyThrust(float deltaTime) {
    if (!isThrusting) return;

    Vector2 thrustDirection = {
        trianglePoints[0].x - position.x,
        trianglePoints[0].y - position.y
    };

    // Normalisieren
    float length = sqrtf(thrustDirection.x * thrustDirection.x + thrustDirection.y * thrustDirection.y);
    if (length > 0.0f) {
        thrustDirection.x /= length;
        thrustDirection.y /= length;

        velocity.x += thrustDirection.x * SPACESHIP_ACCELERATION * deltaTime;
        velocity.y += thrustDirection.y * SPACESHIP_ACCELERATION * deltaTime;

        // Geschwindigkeitsbegrenzung
        float speed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
        if (speed > MAX_SPACESHIP_SPEED) {
            velocity.x = (velocity.x / speed) * MAX_SPACESHIP_SPEED;
            velocity.y = (velocity.y / speed) * MAX_SPACESHIP_SPEED;
        }
    }
}

void Spaceship::StartThrust() { isThrusting = true; }
void Spaceship::StopThrust() { isThrusting = false; }

void Spaceship::Rotate(float direction, float deltaTime) {
    rotation += direction * SPACESHIP_ROTATION_SPEED * deltaTime;
}

// Schild-Methoden
void Spaceship::ActivateShield() {
    shieldActive = true;
    shieldTimer = 8.0f; // 8 Sekunden Schild-Dauer
    shieldAnimationTimer = 0.0f;
}

void Spaceship::DeactivateShield() {
    shieldActive = false;
    shieldTimer = 0.0f;
}

bool Spaceship::IsShieldActive() const {
    return shieldActive;
}

float Spaceship::GetShieldTimeRemaining() const {
    return shieldTimer;
}

// Getter-Methoden
Vector2 Spaceship::GetPosition() const { return position; }
float Spaceship::GetRotation() const { return rotation; }
Vector2 Spaceship::GetVelocity() const { return velocity; }
void Spaceship::setInvulnerableTimer(float value) { invulnerabilityTimer = 3.0f; }
void Spaceship::setIsInvulnerable(bool value) { invulnerable = value; }
bool Spaceship::IsInvulnerable() const { return invulnerable; }
float Spaceship::GetInvulnerabilityTime() const { return invulnerabilityTimer; }  // Neue Methode
bool Spaceship::IsThrusting() const { return isThrusting; }
int Spaceship::GetLives() const { return lives; }
void Spaceship::AddLife() { lives++; }

Rectangle Spaceship::GetBounds() const {
    return { position.x - triangleSize, position.y - triangleSize,
             triangleSize * 2.0f, triangleSize * 2.0f };
}

void Spaceship::LoseLife() {
    lives--;
    // ZUERST Unverwundbarkeit setzen
    invulnerable = true;
    invulnerabilityTimer = 3.0f; // 3 Sekunden Unverwundbarkeit nach Respawn

    // Schild deaktivieren wenn Leben verloren wird
    DeactivateShield();

    // DANN erst Reset() - aber ohne die Unverwundbarkeit zu überschreiben
    Reset();
}