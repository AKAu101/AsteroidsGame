#include "raylib.h"
#include <vector>
#include <cmath>

// Constants
const int screenWidth = 800;
const int screenHeight = 600;
const int maxAsteroids = 10;
const float playerSpeed = 5.0f;
const float playerRotationSpeed = 5.0f;
const float bulletSpeed = 10.0f;
const int maxBullets = 10;

// Player structure
struct Player {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    Vector2 hitbox[3];
    bool active;
};

// Asteroid structure
struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float size;
    bool active;
};

// Bullet structure
struct Bullet {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool active;
    float lifetime;
};

// Game state
struct Game {
    Player player;
    std::vector<Asteroid> asteroids;
    std::vector<Bullet> bullets;
    int score;
    bool gameOver;
};

// Initialize player
void InitPlayer(Player& player) {
    player.position = { static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f };
    player.velocity = { 0.0f, 0.0f };
    player.rotation = 0.0f;
    player.active = true;
}

// Initialize asteroid
Asteroid InitAsteroid() {
    Asteroid asteroid;

    // Position at screen edge
    if (GetRandomValue(0, 1) == 0) {
        asteroid.position.x = static_cast<float>(GetRandomValue(0, screenWidth));
        asteroid.position.y = (GetRandomValue(0, 1) == 0) ? 0.0f : static_cast<float>(screenHeight);
    }
    else {
        asteroid.position.x = (GetRandomValue(0, 1) == 0 ? 0.0f : static_cast<float>(screenWidth));
        asteroid.position.y = static_cast<float>(GetRandomValue(0, screenHeight));
    }

    // Velocity towards screen center
    Vector2 direction = { static_cast<float>(screenWidth) / 2.0f - asteroid.position.x,
                         static_cast<float>(screenHeight) / 2.0f - asteroid.position.y };
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;

    asteroid.velocity = { direction.x * static_cast<float>(GetRandomValue(1, 3)),
                         direction.y * static_cast<float>(GetRandomValue(1, 3)) };
    asteroid.rotation = static_cast<float>(GetRandomValue(0, 360));
    asteroid.size = static_cast<float>(GetRandomValue(20, 50));
    asteroid.active = true;

    return asteroid;
}

// Initialize bullet
Bullet InitBullet(Vector2 position, float rotation) {
    Bullet bullet;
    bullet.position = position;
    bullet.rotation = rotation;

    // Velocity based on rotation
    float rad = rotation * DEG2RAD;
    bullet.velocity = { sinf(rad) * bulletSpeed, -cosf(rad) * bulletSpeed };

    bullet.active = true;
    bullet.lifetime = 60.0f; // 60 frames (~1 second)

    return bullet;
}

// Update player
void UpdatePlayer(Player& player) {
    if (!player.active) return;

    // Rotation
    if (IsKeyDown(KEY_LEFT)) player.rotation -= playerRotationSpeed;
    if (IsKeyDown(KEY_RIGHT)) player.rotation += playerRotationSpeed;

    // Acceleration
    if (IsKeyDown(KEY_UP)) {
        float rad = player.rotation * DEG2RAD;
        player.velocity.x += sinf(rad) * 0.2f;
        player.velocity.y -= cosf(rad) * 0.2f;
    }

    // Movement
    player.position.x += player.velocity.x;
    player.position.y += player.velocity.y;

    // Screen wrapping
    if (player.position.x < 0.0f) player.position.x = static_cast<float>(screenWidth);
    if (player.position.x > static_cast<float>(screenWidth)) player.position.x = 0.0f;
    if (player.position.y < 0.0f) player.position.y = static_cast<float>(screenHeight);
    if (player.position.y > static_cast<float>(screenHeight)) player.position.y = 0.0f;

    // Friction
    player.velocity.x *= 0.98f;
    player.velocity.y *= 0.98f;
}

// Update asteroids
void UpdateAsteroids(std::vector<Asteroid>& asteroids) {
    for (auto& asteroid : asteroids) {
        if (!asteroid.active) continue;

        // Movement
        asteroid.position.x += asteroid.velocity.x;
        asteroid.position.y += asteroid.velocity.y;
        asteroid.rotation += 0.5f;

        // Screen wrapping
        if (asteroid.position.x < -asteroid.size) asteroid.position.x = static_cast<float>(screenWidth) + asteroid.size;
        if (asteroid.position.x > static_cast<float>(screenWidth) + asteroid.size) asteroid.position.x = -asteroid.size;
        if (asteroid.position.y < -asteroid.size) asteroid.position.y = static_cast<float>(screenHeight) + asteroid.size;
        if (asteroid.position.y > static_cast<float>(screenHeight) + asteroid.size) asteroid.position.y = -asteroid.size;
    }
}

// Update bullets
void UpdateBullets(std::vector<Bullet>& bullets) {
    for (auto& bullet : bullets) {
        if (!bullet.active) continue;

        // Movement
        bullet.position.x += bullet.velocity.x;
        bullet.position.y += bullet.velocity.y;
        bullet.lifetime -= 1.0f;

        // Deactivate when lifetime expires or off-screen
        if (bullet.lifetime <= 0.0f ||
            bullet.position.x < 0.0f || bullet.position.x > static_cast<float>(screenWidth) ||
            bullet.position.y < 0.0f || bullet.position.y > static_cast<float>(screenHeight)) {
            bullet.active = false;
        }
    }
}

// Check collisions
void CheckCollisions(Game& game) {
    // Player with asteroids
    for (auto& asteroid : game.asteroids) {
        if (!asteroid.active || !game.player.active) continue;

        float distance = sqrtf(powf(game.player.position.x - asteroid.position.x, 2.0f) +
            powf(game.player.position.y - asteroid.position.y, 2.0f));

        if (distance < asteroid.size) {
            game.player.active = false;
            game.gameOver = true;
        }
    }

    // Bullets with asteroids
    for (auto& bullet : game.bullets) {
        if (!bullet.active) continue;

        for (auto& asteroid : game.asteroids) {
            if (!asteroid.active) continue;

            float distance = sqrtf(powf(bullet.position.x - asteroid.position.x, 2.0f) +
                powf(bullet.position.y - asteroid.position.y, 2.0f));

            if (distance < asteroid.size) {
                bullet.active = false;
                asteroid.active = false;
                game.score += 100;

                // Spawn new asteroid if too few remain
                if (game.asteroids.size() < static_cast<size_t>(maxAsteroids) && GetRandomValue(0, 100) > 70) {
                    game.asteroids.push_back(InitAsteroid());
                }
            }
        }
    }
}

// Draw player
void DrawPlayer(const Player& player) {
    if (!player.active) return;

    Vector2 v1 = { player.position.x + sinf(player.rotation * DEG2RAD) * 20.0f,
                   player.position.y - cosf(player.rotation * DEG2RAD) * 20.0f };
    Vector2 v2 = { player.position.x - sinf(player.rotation * DEG2RAD + 0.8f) * 10.0f,
                   player.position.y + cosf(player.rotation * DEG2RAD + 0.8f) * 10.0f };
    Vector2 v3 = { player.position.x - sinf(player.rotation * DEG2RAD - 0.8f) * 10.0f,
                   player.position.y + cosf(player.rotation * DEG2RAD - 0.8f) * 10.0f };

    DrawTriangle(v1, v2, v3, WHITE);
}

// Draw asteroids
void DrawAsteroids(const std::vector<Asteroid>& asteroids) {
    for (const auto& asteroid : asteroids) {
        if (!asteroid.active) continue;

        DrawCircleLines(static_cast<int>(asteroid.position.x),
            static_cast<int>(asteroid.position.y),
            static_cast<int>(asteroid.size), GRAY);
    }
}

// Draw bullets
void DrawBullets(const std::vector<Bullet>& bullets) {
    for (const auto& bullet : bullets) {
        if (!bullet.active) continue;

        DrawCircle(static_cast<int>(bullet.position.x),
            static_cast<int>(bullet.position.y),
            3, YELLOW);
    }
}

int main() {
    // Initialize window
    InitWindow(screenWidth, screenHeight, "Asteroids with Raylib");
    SetTargetFPS(60);

    // Initialize game
    Game game;
    InitPlayer(game.player);
    game.score = 0;
    game.gameOver = false;

    // Create asteroids
    for (int i = 0; i < maxAsteroids / 2; i++) {
        game.asteroids.push_back(InitAsteroid());
    }

    // Main game loop
    while (!WindowShouldClose()) {
        // Update game
        if (!game.gameOver) {
            UpdatePlayer(game.player);
            UpdateAsteroids(game.asteroids);
            UpdateBullets(game.bullets);
            CheckCollisions(game);

            // Fire bullet
            if (IsKeyPressed(KEY_SPACE) && game.bullets.size() < static_cast<size_t>(maxBullets)) {
                game.bullets.push_back(InitBullet(game.player.position, game.player.rotation));
            }

            // Spawn new asteroids if too few remain
            int activeAsteroids = 0;
            for (const auto& asteroid : game.asteroids) {
                if (asteroid.active) activeAsteroids++;
            }

            if (activeAsteroids < maxAsteroids / 2 && GetRandomValue(0, 100) > 95) {
                game.asteroids.push_back(InitAsteroid());
            }
        }
        else {
            // Restart game
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

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        DrawPlayer(game.player);
        DrawAsteroids(game.asteroids);
        DrawBullets(game.bullets);

        // Draw score
        DrawText(TextFormat("Score: %d", game.score), 10, 10, 20, WHITE);

        // Draw Game Over
        if (game.gameOver) {
            DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, screenHeight / 2 - 40, 40, RED);
            DrawText("Press R to restart", screenWidth / 2 - MeasureText("Press R to restart", 20) / 2, screenHeight / 2 + 20, 20, WHITE);
        }
        EndDrawing();
    }

    // Cleanup
    CloseWindow();
    return 0;
}