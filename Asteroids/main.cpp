#include "raylib.h"
#include <vector>
#include <cmath>

// Konstanten
const int screenWidth = 800;
const int screenHeight = 600;
const int maxAsteroids = 10;
const float playerSpeed = 5.0f;
const float playerRotationSpeed = 5.0f;
const float bulletSpeed = 10.0f;
const int maxBullets = 10;

// Spieler-Struktur
struct Player {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    Vector2 hitbox[3];
    bool active;
};

// Asteroid-Struktur
struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float size;
    bool active;
};

// Projektil-Struktur
struct Bullet {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool active;
    float lifetime;
};

// Spielzustand
struct Game {
    Player player;
    std::vector<Asteroid> asteroids;
    std::vector<Bullet> bullets;
    int score;
    bool gameOver;
};

// Initialisiert den Spieler
void InitPlayer(Player& player) {
    player.position = { screenWidth / 2.0f, screenHeight / 2.0f };
    player.velocity = { 0.0f, 0.0f };
    player.rotation = 0.0f;
    player.active = true;
}

// Initialisiert einen Asteroiden
Asteroid InitAsteroid() {
    Asteroid asteroid;

    // Position am Rand des Bildschirms
    if (GetRandomValue(0, 1) == 0) {
        asteroid.position.x = GetRandomValue(0, screenWidth);
        asteroid.position.y = ((GetRandomValue(0, 1) == 0) ? 0 : screenHeight));
    }
    else {
        asteroid.position.x = ((GetRandomValue(0, 1) == 0) ? 0 : screenWidth));
        asteroid.position.y = GetRandomValue(0, screenHeight);
    }

    // Geschwindigkeit in Richtung Bildschirmmitte
    Vector2 direction = { screenWidth / 2.0f - asteroid.position.x, screenHeight / 2.0f - asteroid.position.y };
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;

    asteroid.velocity = { direction.x * GetRandomValue(1, 3), direction.y * GetRandomValue(1, 3) };
    asteroid.rotation = GetRandomValue(0, 360);
    asteroid.size = GetRandomValue(20, 50);
    asteroid.active = true;

    return asteroid;
}

// Initialisiert ein Projektil
Bullet InitBullet(Vector2 position, float rotation) {
    Bullet bullet;
    bullet.position = position;
    bullet.rotation = rotation;

    // Geschwindigkeit basierend auf der Rotation
    float rad = rotation * DEG2RAD;
    bullet.velocity = { sin(rad) * bulletSpeed, -cos(rad) * bulletSpeed };

    bullet.active = true;
    bullet.lifetime = 60; // 60 Frames (~1 Sekunde)

    return bullet;
}

// Aktualisiert den Spieler
void UpdatePlayer(Player& player) {
    if (!player.active) return;

    // Rotation
    if (IsKeyDown(KEY_LEFT)) player.rotation -= playerRotationSpeed;
    if (IsKeyDown(KEY_RIGHT)) player.rotation += playerRotationSpeed;

    // Beschleunigung
    if (IsKeyDown(KEY_UP)) {
        float rad = player.rotation * DEG2RAD;
        player.velocity.x += sin(rad) * 0.2f;
        player.velocity.y -= cos(rad) * 0.2f;
    }

    // Bewegung
    player.position.x += player.velocity.x;
    player.position.y += player.velocity.y;

    // Bildschirmbegrenzung
    if (player.position.x < 0) player.position.x = screenWidth;
    if (player.position.x > screenWidth) player.position.x = 0;
    if (player.position.y < 0) player.position.y = screenHeight;
    if (player.position.y > screenHeight) player.position.y = 0;

    // Reibung
    player.velocity.x *= 0.98f;
    player.velocity.y *= 0.98f;
}

// Aktualisiert die Asteroiden
void UpdateAsteroids(std::vector<Asteroid>& asteroids) {
    for (auto& asteroid : asteroids) {
        if (!asteroid.active) continue;

        // Bewegung
        asteroid.position.x += asteroid.velocity.x;
        asteroid.position.y += asteroid.velocity.y;
        asteroid.rotation += 0.5f;

        // Bildschirmbegrenzung
        if (asteroid.position.x < -asteroid.size) asteroid.position.x = screenWidth + asteroid.size;
        if (asteroid.position.x > screenWidth + asteroid.size) asteroid.position.x = -asteroid.size;
        if (asteroid.position.y < -asteroid.size) asteroid.position.y = screenHeight + asteroid.size;
        if (asteroid.position.y > screenHeight + asteroid.size) asteroid.position.y = -asteroid.size;
    }
}

// Aktualisiert die Projektile
void UpdateBullets(std::vector<Bullet>& bullets) {
    for (auto& bullet : bullets) {
        if (!bullet.active) continue;

        // Bewegung
        bullet.position.x += bullet.velocity.x;
        bullet.position.y += bullet.velocity.y;
        bullet.lifetime--;

        // Deaktivieren wenn Lebenszeit abgelaufen oder außerhalb des Bildschirms
        if (bullet.lifetime <= 0 ||
            bullet.position.x < 0 || bullet.position.x > screenWidth ||
            bullet.position.y < 0 || bullet.position.y > screenHeight) {
            bullet.active = false;
        }
    }
}

// Überprüft Kollisionen
void CheckCollisions(Game& game) {
    // Spieler mit Asteroiden
    for (auto& asteroid : game.asteroids) {
        if (!asteroid.active || !game.player.active) continue;

        float distance = sqrt(pow(game.player.position.x - asteroid.position.x, 2) +
            pow(game.player.position.y - asteroid.position.y, 2));

        if (distance < asteroid.size) {
            game.player.active = false;
            game.gameOver = true;
        }
    }

    // Projektile mit Asteroiden
    for (auto& bullet : game.bullets) {
        if (!bullet.active) continue;

        for (auto& asteroid : game.asteroids) {
            if (!asteroid.active) continue;

            float distance = sqrt(pow(bullet.position.x - asteroid.position.x, 2) +
                pow(bullet.position.y - asteroid.position.y, 2));

            if (distance < asteroid.size) {
                bullet.active = false;
                asteroid.active = false;
                game.score += 100;

                // Neuen Asteroiden spawnen, wenn zu wenige vorhanden sind
                if (game.asteroids.size() < maxAsteroids && GetRandomValue(0, 100) > 70) {
                    game.asteroids.push_back(InitAsteroid());
                }
            }
        }
    }
}

// Zeichnet den Spieler
void DrawPlayer(const Player& player) {
    if (!player.active) return;

    Vector2 v1 = { player.position.x + sin(player.rotation * DEG2RAD) * 20.0f,
                   player.position.y - cos(player.rotation * DEG2RAD) * 20.0f };
    Vector2 v2 = { player.position.x - sin(player.rotation * DEG2RAD + 0.8f) * 10.0f,
                   player.position.y + cos(player.rotation * DEG2RAD + 0.8f) * 10.0f };
    Vector2 v3 = { player.position.x - sin(player.rotation * DEG2RAD - 0.8f) * 10.0f,
                   player.position.y + cos(player.rotation * DEG2RAD - 0.8f) * 10.0f };

    DrawTriangle(v1, v2, v3, WHITE);
}

// Zeichnet die Asteroiden
void DrawAsteroids(const std::vector<Asteroid>& asteroids) {
    for (const auto& asteroid : asteroids) {
        if (!asteroid.active) continue;

        DrawCircleLines(asteroid.position.x, asteroid.position.y, asteroid.size, GRAY);
    }
}

// Zeichnet die Projektile
void DrawBullets(const std::vector<Bullet>& bullets) {
    for (const auto& bullet : bullets) {
        if (!bullet.active) continue;

        DrawCircle(bullet.position.x, bullet.position.y, 3, YELLOW);
    }
}

int main() {
    // Fenster initialisieren
    InitWindow(screenWidth, screenHeight, "Asteroids mit Raylib");
    SetTargetFPS(60);

    // Spiel initialisieren
    Game game;
    InitPlayer(game.player);
    game.score = 0;
    game.gameOver = false;

    // Asteroiden erstellen
    for (int i = 0; i < maxAsteroids / 2; i++) {
        game.asteroids.push_back(InitAsteroid());
    }

    // Hauptspielschleife
    while (!WindowShouldClose()) {
        // Spiel aktualisieren
        if (!game.gameOver) {
            UpdatePlayer(game.player);
            UpdateAsteroids(game.asteroids);
            UpdateBullets(game.bullets);
            CheckCollisions(game);

            // Projektil abfeuern
            if (IsKeyPressed(KEY_SPACE) && game.bullets.size() < maxBullets) {
                game.bullets.push_back(InitBullet(game.player.position, game.player.rotation));
            }

            // Neue Asteroiden spawnen, wenn zu wenige vorhanden sind
            int activeAsteroids = 0;
            for (const auto& asteroid : game.asteroids) {
                if (asteroid.active) activeAsteroids++;
            }

            if (activeAsteroids < maxAsteroids / 2 && GetRandomValue(0, 100) > 95) {
                game.asteroids.push_back(InitAsteroid());
            }
        }
        else {
            // Spiel neustarten
            if (IsKeyPressed(KEY_R)) {
                game.player.active = true;
                game.asteroids.clear();
                game.bullets.clear();
                game.score = 0;
                game.gameOver = false;

                for (int i = 0; i < maxAsteroids / 2; i++) {
                    game.asteroids.push_back(InitAsteroid());
                }
            }
        }

        // Zeichnen
        BeginDrawing();
        ClearBackground(BLACK);

        DrawPlayer(game.player);
        DrawAsteroids(game.asteroids);
        DrawBullets(game.bullets);

        // Punkte anzeigen
        DrawText(TextFormat("Punkte: %d", game.score), 10, 10, 20, WHITE);

        // Game Over anzeigen
        if (game.gameOver) {
            DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, screenHeight / 2 - 40, 40, RED);
            DrawText("Drücke R zum Neustarten", screenWidth / 2 - MeasureText("Drücke R zum Neustarten", 20) / 2, screenHeight / 2 + 20, 20, WHITE);
        }
        EndDrawing();
    }

    // Aufräumen
    CloseWindow();
    return 0;
}