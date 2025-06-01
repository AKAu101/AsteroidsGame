#include "game.h"

Game::Game() :
    gameRunning(true),
    projectileCooldown(0),
    asteroidSpawnTimer(0),
    currentItem(0),
    amountRapid(0),
    hasRapid(false),
    hasShield(false),
    stateManager(),
    player(),
    gameScore(),
    objectManager(player),
    collisionSystem(objectManager, gameScore),
    inputHandler(stateManager, objectManager, projectileCooldown,
        currentItem, hasRapid, amountRapid, hasShield),
    uiRenderer(stateManager, gameScore) {
    InitGameSounds();
}

Game::~Game() {
    gameScore.SaveHighScore();
    UnloadGameSounds();
}

void Game::Initialize() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids");
    SetTargetFPS(60);
    objectManager.SpawnAsteroids(4);
}

void Game::Update() {
    float deltaTime = GetFrameTime();

    inputHandler.HandleInput(deltaTime);
    stateManager.Update(deltaTime);

    if (stateManager.GetCurrentState() == IN_GAME) {
        UpdateInGame(deltaTime);
    }
}

void Game::UpdateInGame(float deltaTime) {
    player.Update(deltaTime);
    objectManager.UpdateObjects(deltaTime);
    UpdateTimers(deltaTime);
    bool isInvulnerable = player.IsInvulnerable();
    collisionSystem.CheckCollisions(hasShield, isInvulnerable);
    HandleSpawning(deltaTime);
    CheckGameState();
}

void Game::Draw() {
    BeginDrawing();
    uiRenderer.DrawCurrentState();
    EndDrawing();
}

void Game::InitGameSounds() {
    InitAudioDevice();
    shootSound = LoadSound("Sounds/shoot.wav");
}

void Game::UnloadGameSounds() {
    UnloadSound(shootSound);
    CloseAudioDevice();
}

void Game::UpdateTimers(float deltaTime) {
    if (projectileCooldown > 0) projectileCooldown -= deltaTime;
    asteroidSpawnTimer += deltaTime;
}

void Game::HandleSpawning(float deltaTime) {
    if (asteroidSpawnTimer > 10.0f) {
        objectManager.SpawnAsteroid(objectManager.GetRandomEdgePosition(), LARGE);
        asteroidSpawnTimer = 0;
    }
}

void Game::CheckGameState() {
    bool asteroidsActive = false;
    for (const auto& asteroid : objectManager.GetAsteroids()) {
        if (asteroid.IsActive()) {
            asteroidsActive = true;
            break;
        }
    }

    if (!asteroidsActive) {
        objectManager.SpawnAsteroids(4 + gameScore.GetLevel());
    }

    if (player.GetLives() <= 0) {
        stateManager.SetState(GAME_OVER);
    }
}

void Game::ResetGame() {
    gameScore.Reset();
    player = Spaceship();
    objectManager.ResetObjects();
    projectileCooldown = 0;
    asteroidSpawnTimer = 0;
    hasRapid = false;
    hasShield = false;
    currentItem = 0;
    objectManager.SpawnAsteroids(4);
}