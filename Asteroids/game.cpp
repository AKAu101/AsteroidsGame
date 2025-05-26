#include "game.h"
#include "globals.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

Game::Game() {
    gameRunning = true;
    currentState = MAIN_MENU;
    menuSelection = 0;
    projectileCooldown = 0;
    asteroidSpawnTimer = 0;
    srand(static_cast<unsigned int>(time(nullptr)));
}

Game::~Game() {
    gameScore.SaveHighScore();
}

void Game::Initialize() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids");
    SetTargetFPS(60);

    // Initialize vectors with proper size
    asteroids.resize(MAX_ASTEROIDS);
    projectiles.resize(MAX_PROJECTILES);
    powerups.resize(10);

    // Initialize game objects
    SpawnAsteroids(4);
}

void Game::Update() {
    float deltaTime = GetFrameTime();

    // PHASE 1: Input handling (nur hier!)
    HandleInput(deltaTime);

    switch (currentState) {
    case MAIN_MENU:
        UpdateMainMenu();
        break;
    case IN_GAME:
        UpdateInGame(deltaTime);
        break;
    case GAME_OVER:
        UpdateGameOver();
        break;
    case OPTIONS:
        // Options menu implementation
        break;
    }
}

// PHASE 1: Zentrales Input-Handling
void Game::HandleInput(float deltaTime) {
    // Globale Navigation
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentState == IN_GAME) {
            currentState = MAIN_MENU;
        }
        else if (currentState == OPTIONS) {
            currentState = MAIN_MENU;
        }
        else if (currentState == MAIN_MENU) {
            gameRunning = false;
        }
    }

    // Spiel-spezifisches Input
    if (currentState == IN_GAME) {
        HandleGameInput(deltaTime);
    }
}

void Game::HandleGameInput(float deltaTime) {
    // Player Movement - klare Aktionen
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        player.StartThrust();
        player.ApplyThrust(deltaTime);
    }
    else {
        player.StopThrust();
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        player.Rotate(-1, deltaTime);
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        player.Rotate(1, deltaTime);
    }

    // Shooting
    if (IsKeyPressed(KEY_SPACE) && projectileCooldown <= 0) {
        FireProjectile();
    }
}

void Game::UpdateMainMenu() {
    if (IsKeyPressed(KEY_UP)) {
        menuSelection = (menuSelection - 1 + 3) % 3;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        menuSelection = (menuSelection + 1) % 3;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        switch (menuSelection) {
        case 0: // Start Game
            currentState = IN_GAME;
            ResetGame();
            break;
        case 1: // Options
            currentState = OPTIONS;
            break;
        case 2: // Quit
            gameRunning = false;
            break;
        }
    }
}

// PHASE 2: Game Logic Update (kein Input!)
void Game::UpdateInGame(float deltaTime) {
    // Update all objects (pure physics/state)
    player.Update(deltaTime);

    for (auto& projectile : projectiles) {
        projectile.Update(deltaTime);
    }

    for (auto& asteroid : asteroids) {
        asteroid.Update(deltaTime);
    }

    for (auto& powerup : powerups) {
        powerup.Update(deltaTime);
    }

    // Update timers
    UpdateTimers(deltaTime);

    // PHASE 3: Game Logic
    CheckCollisions();
    HandleSpawning(deltaTime);
    CheckGameState();
}

void Game::UpdateTimers(float deltaTime) {
    if (projectileCooldown > 0) {
        projectileCooldown -= deltaTime;
    }

    asteroidSpawnTimer += deltaTime;
}

void Game::HandleSpawning(float deltaTime) {
    // Spawn new asteroids periodically
    if (asteroidSpawnTimer > 10.0f) {
        SpawnAsteroid(GetRandomEdgePosition(), LARGE);
        asteroidSpawnTimer = 0;
    }
}

void Game::CheckGameState() {
    // Check if all asteroids are destroyed
    bool hasActiveAsteroids = false;
    for (const auto& asteroid : asteroids) {
        if (asteroid.IsActive()) {
            hasActiveAsteroids = true;
            break;
        }
    }

    if (!hasActiveAsteroids) {
        SpawnAsteroids(4 + gameScore.GetLevel());
    }

    // Check game over
    if (player.GetLives() <= 0) {
        currentState = GAME_OVER;
        gameScore.SaveHighScore();
    }
}

void Game::FireProjectile() {
    for (auto& projectile : projectiles) {
        if (!projectile.IsActive()) {
            projectile.Fire(player.GetPosition(), player.GetRotation());
            projectileCooldown = 0.2f;
            break;
        }
    }
}

void Game::UpdateGameOver() {
    if (IsKeyPressed(KEY_ENTER)) {
        currentState = MAIN_MENU;
    }
}

// PHASE 4: Drawing (nur const Methoden)
void Game::Draw() {
    BeginDrawing();

    // Background depends on game state
    if (currentState == IN_GAME) {
        DrawCheckeredBackground();
    }
    else {
        ClearBackground(BLACK);
    }

    switch (currentState) {
    case MAIN_MENU:
        DrawMainMenu();
        break;
    case IN_GAME:
        DrawInGame();
        break;
    case GAME_OVER:
        DrawGameOver();
        break;
    case OPTIONS:
        DrawText("OPTIONS MENU", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 20, WHITE);
        DrawText("Press ESC to return", SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 50, 10, WHITE);
        break;
    }

    EndDrawing();
}
void Game::DrawCheckeredBackground() {
    // MS Paint default background
    ClearBackground(WHITE); // White

    // Draw grid lines
    for (int x = 0; x < SCREEN_WIDTH; x += 40) {
        DrawLine(x, 0, x, SCREEN_HEIGHT, LIGHTGRAY); // Silver lines
    }
    for (int y = 0; y < SCREEN_HEIGHT; y += 40) {
        DrawLine(0, y, SCREEN_WIDTH, y, LIGHTGRAY); // Silver lines
    }
}


void Game::DrawMainMenu() {
    // MS Paint-ähnlicher Hintergrund
    ClearBackground(MSPAINT_COLORS[3]); // Weiß
    DrawRectangle(10, 10, SCREEN_WIDTH-20, SCREEN_HEIGHT-20, MSPAINT_COLORS[2]); // Hellgrauer Rahmen
    
    // Titel mit "ausgefranster" Schrift
    const char* title = "ASTEROIDS";
    int titleWidth = MeasureText(title, 40);
    for (int i = 0; i < titleWidth; i += 2) {
        DrawText(title, SCREEN_WIDTH/2 - titleWidth/2 + GetRandomValue(-1,1), 
                200 + GetRandomValue(-1,1), 40, BLACK);
    }

    // 3D-Buttons wie in MS Paint
    const char* menuItems[] = { "START GAME", "OPTIONS", "QUIT" };
    for (int i = 0; i < 3; i++) {
        // Button-Rahmen
        DrawRectangle(SCREEN_WIDTH/2 - 100, 350 + i*60, 200, 40, MSPAINT_COLORS[2]);
        if (i == menuSelection) {
            DrawRectangle(SCREEN_WIDTH/2 - 98, 352 + i*60, 196, 36, MSPAINT_COLORS[5]); // Rot wenn ausgewählt
        } else {
            DrawRectangle(SCREEN_WIDTH/2 - 98, 352 + i*60, 196, 36, MSPAINT_COLORS[3]); // Weiß
        }
        
        // Button-Text
        int textWidth = MeasureText(menuItems[i], 20);
        DrawText(menuItems[i], SCREEN_WIDTH/2 - textWidth/2, 360 + i*60, 20, BLACK);
        
        // 3D-Effekt
        DrawLine(SCREEN_WIDTH/2 - 100, 350 + i*60, SCREEN_WIDTH/2 + 100, 350 + i*60, MSPAINT_COLORS[0]); // Oben schwarz
        DrawLine(SCREEN_WIDTH/2 - 100, 350 + i*60, SCREEN_WIDTH/2 - 100, 390 + i*60, MSPAINT_COLORS[0]); // Links schwarz
        DrawLine(SCREEN_WIDTH/2 + 100, 350 + i*60, SCREEN_WIDTH/2 + 100, 390 + i*60, MSPAINT_COLORS[3]); // Rechts weiß
        DrawLine(SCREEN_WIDTH/2 - 100, 390 + i*60, SCREEN_WIDTH/2 + 100, 390 + i*60, MSPAINT_COLORS[3]); // Unten weiß
    }
}

void Game::DrawInGame() {
    // Draw all objects (const methods only)
    player.Draw();

    for (const auto& projectile : projectiles) {
        projectile.Draw();
    }

    for (const auto& asteroid : asteroids) {
        asteroid.Draw();
    }

    for (const auto& powerup : powerups) {
        powerup.Draw();
    }

    // Draw UI with dark background for better readability
    DrawRectangle(5, 5, 200, 100, Color{ 0, 0, 0, 150 }); // Semi-transparent black background
    DrawText(("SCORE: " + std::to_string(gameScore.GetScore())).c_str(), 10, 10, 20, BLACK);
    DrawText(("LIVES: " + std::to_string(player.GetLives())).c_str(), 10, 40, 20, BLACK);
    DrawText(("LEVEL: " + std::to_string(gameScore.GetLevel())).c_str(), 10, 70, 20, BLACK);
}

void Game::DrawGameOver() {
    DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 30, RED);
    DrawText(("SCORE: " + std::to_string(gameScore.GetScore())).c_str(),
        SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 20, WHITE);
    DrawText(("HIGH SCORE: " + std::to_string(gameScore.GetHighScore())).c_str(),
        SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 30, 20, WHITE);
    DrawText("Press ENTER to return to menu", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 80, 15, WHITE);
}

void Game::SpawnAsteroids(int count) {
    for (int i = 0; i < count; i++) {
        SpawnAsteroid(GetRandomEdgePosition(), LARGE);
    }
}

void Game::SpawnAsteroid(Vector2 position, AsteroidSize size) {
    for (auto& asteroid : asteroids) {
        if (!asteroid.IsActive()) {
            asteroid.Spawn(position, size);
            return;
        }
    }
}

void Game::CheckCollisions() {
    // Player vs Asteroids
    if (!player.IsInvulnerable()) {
        for (auto& asteroid : asteroids) {
            if (asteroid.IsActive() &&
                CheckCircleCollision(player.GetPosition(), 10,
                    asteroid.GetPosition(), asteroid.GetRadius())) {
                player.LoseLife();
                break;
            }
        }
    }

    // Projectiles vs Asteroids
    for (auto& projectile : projectiles) {
        if (!projectile.IsActive()) continue;

        for (size_t i = 0; i < asteroids.size(); i++) {
            if (!asteroids[i].IsActive()) continue;

            if (CheckCircleCollision(projectile.GetPosition(), 3,
                asteroids[i].GetPosition(), asteroids[i].GetRadius())) {
                gameScore.AddPoints(asteroids[i].GetPoints());
                HandleAsteroidDestruction(static_cast<int>(i));
                projectile.Deactivate();
                break;
            }
        }
    }

    // Player vs PowerUps
    for (auto& powerup : powerups) {
        if (powerup.IsActive() &&
            CheckCircleCollision(player.GetPosition(), 10,
                powerup.GetPosition(), 25)) {
            switch (powerup.GetType()) {
            case EXTRA_LIFE:
                player.AddLife();
                break;
            case RAPID_FIRE:
                // Implement rapid fire
                break;
            case SHIELD:
                // Implement shield
                break;
            }
            powerup.Collect();
        }
    }
}

void Game::HandleAsteroidDestruction(int asteroidIndex) {
    Asteroid& asteroid = asteroids[asteroidIndex];
    Vector2 pos = asteroid.GetPosition();
    AsteroidSize size = asteroid.GetSize();

    asteroid.Destroy();

    // Spawn smaller asteroids
    if (size == LARGE) {
        SpawnAsteroid({ pos.x + 20, pos.y }, MEDIUM);
        SpawnAsteroid({ pos.x - 20, pos.y }, MEDIUM);
    }
    else if (size == MEDIUM) {
        SpawnAsteroid({ pos.x + 10, pos.y }, SMALL);
        SpawnAsteroid({ pos.x - 10, pos.y }, SMALL);
    }

    // Chance to spawn powerup
    if (rand() / static_cast<float>(RAND_MAX) < POWERUP_SPAWN_RATE) {
        SpawnPowerUp(pos);
    }
}

void Game::SpawnPowerUp(Vector2 position) {
    for (auto& powerup : powerups) {
        if (!powerup.IsActive()) {
            PowerUpType type = static_cast<PowerUpType>(rand() % 3);
            powerup.Spawn(position, type);
            return;
        }
    }
}

bool Game::CheckCircleCollision(Vector2 pos1, float radius1, Vector2 pos2, float radius2) {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (radius1 + radius2);
}

void Game::ResetGame() {
    gameScore.Reset();
    player = Spaceship();

    // Clear all objects
    for (auto& asteroid : asteroids) {
        asteroid.Destroy();
    }
    for (auto& projectile : projectiles) {
        projectile.Deactivate();
    }
    for (auto& powerup : powerups) {
        powerup.Collect();
    }

    // Reset timers
    projectileCooldown = 0;
    asteroidSpawnTimer = 0;

    // Spawn initial asteroids
    SpawnAsteroids(4);
}

Vector2 Game::GetRandomEdgePosition() {
    Vector2 pos = { 0, 0 }; // Explizite Initialisierung
    int side = rand() % 4; // 0=top, 1=right, 2=bottom, 3=left

    switch (side) {
    case 0: // Top
        pos.x = static_cast<float>(rand() % SCREEN_WIDTH);
        pos.y = -50.0f;
        break;
    case 1: // Right
        pos.x = SCREEN_WIDTH + 50.0f;
        pos.y = static_cast<float>(rand() % SCREEN_HEIGHT);
        break;
    case 2: // Bottom
        pos.x = static_cast<float>(rand() % SCREEN_WIDTH);
        pos.y = SCREEN_HEIGHT + 50.0f;
        break;
    case 3: // Left
        pos.x = -50.0f;
        pos.y = static_cast<float>(rand() % SCREEN_HEIGHT);
        break;
    }
    return pos;
}