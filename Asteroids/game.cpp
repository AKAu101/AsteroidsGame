#include "game.h"
#include "globals.h"
#include "highscoreManager.h"
#include <iostream>

Game::Game() :
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
    inputHandler(*this, stateManager, objectManager, projectileCooldown,
        currentItem, hasRapid, amountRapid, hasShield),
    uiRenderer(stateManager, gameScore, player, objectManager, currentItem) {
    InitGameSounds();

    // HighscoreManager mit UIRenderer verknüpfen
    uiRenderer.SetHighscoreManager(&highscoreManager);
}

Game::~Game() {
    gameScore.SaveHighScore();
    UnloadGameSounds();
}

void Game::Initialize() {
    gameRunning = true;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids");
    SetTargetFPS(60);
    objectManager.SpawnAsteroids(4);
}

void Game::Update() {
    float deltaTime = GetFrameTime();

    // Hintergrundmusik updaten
    UpdateMusicStream(backgroundMusic);

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

    // DEBUG: Unverwundbarkeits-Status ausgeben
    static float debugTimer = 0;
    debugTimer += deltaTime;
    if (debugTimer > 1.0f && isInvulnerable) {
        printf("Player is INVULNERABLE for %.1f more seconds\n",
            player.GetInvulnerabilityTime());
        debugTimer = 0;
    }

    collisionSystem.CheckCollisions(hasShield, isInvulnerable);
    HandleSpawning(deltaTime);
    CheckGameState();
    CheckPowerUpCollisions();
}

void Game::CheckPowerUpCollisions() {
    Rectangle playerBounds = player.GetBounds();

    // Debug: Spieler-Position ausgeben
    static float debugTimer2 = 0;
    debugTimer2 += GetFrameTime();
    if (debugTimer2 > 2.0f) {
        printf("Player pos: %.1f, %.1f | PowerUps: %zu\n",
            playerBounds.x, playerBounds.y, objectManager.GetPowerUps().size());
        debugTimer2 = 0;
    }

    // Prüfe alle PowerUps
    auto& powerups = const_cast<std::vector<PowerUp>&>(objectManager.GetPowerUps());

    for (auto& powerup : powerups) {
        if (powerup.IsActive()) {
            Rectangle powerupBounds = powerup.GetBounds();

            if (CheckCollisionRecs(playerBounds, powerupBounds)) {
                printf("COLLISION DETECTED! PowerUp Type: %d\n", (int)powerup.GetType());

                // PowerUp aufsammeln
                PowerUpType type = powerup.GetType();
                powerup.Collect(); // PowerUp deaktivieren

                // Item basierend auf Typ setzen
                switch (type) {
                case EXTRA_LIFE:
                    printf("Collected EXTRA_LIFE - adding life immediately\n");
                    player.AddLife();
                    currentItem = 0; // Sofort verwenden, nicht speichern
                    break;

                case RAPID_FIRE:
                    printf("Collected RAPID_FIRE - stored as item\n");
                    currentItem = 1; // Als Item speichern
                    break;

                case SHIELD:
                    printf("Collected SHIELD - stored as item\n");
                    currentItem = 2; // Als Item speichern
                    break;

                default:
                    printf("Unknown PowerUp type: %d\n", (int)type);
                    break;
                }

                printf("currentItem is now: %d\n", currentItem);
                break; // Nur ein PowerUp pro Frame aufsammeln
            }
        }
    }
}

void Game::Draw() {
    BeginDrawing();
    uiRenderer.DrawCurrentState();
    EndDrawing();
}

void Game::InitGameSounds() {
    InitAudioDevice();
    shootSound = LoadSound("Sounds/shoot.wav");
    SetSoundVolume(shootSound, 0.3f);

    backgroundMusic = LoadMusicStream("Sounds/Soundtrack.ogg");
    SetMusicVolume(backgroundMusic, 0.1f);
    PlayMusicStream(backgroundMusic);
}

void Game::UnloadGameSounds() {
    UnloadSound(shootSound);
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
}

void Game::PlayShootSound() {
    if (shootSound.frameCount > 0) {
        PlaySound(shootSound);
    }
}

void Game::UpdateTimers(float deltaTime) {
    if (projectileCooldown > 0) projectileCooldown -= deltaTime;
    asteroidSpawnTimer += deltaTime;
}

void Game::HandleSpawning(float deltaTime) {
    float baseSpawnTime = 10.0f;
    float minSpawnTime = 3.0f;
    float scoreReduction = gameScore.GetScore() / 1000.0f;
    float currentSpawnTime = SafeMax(minSpawnTime, baseSpawnTime - scoreReduction);

    if (asteroidSpawnTimer > currentSpawnTime) {
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
        int baseAsteroids = 4;
        int bonusAsteroids = gameScore.GetScore() / 2000;
        int maxAsteroids = 10;
        int asteroidsToSpawn = (int)SafeMin((float)maxAsteroids, (float)(baseAsteroids + bonusAsteroids));

        objectManager.SpawnAsteroids(asteroidsToSpawn);
    }

    if (player.GetLives() <= 0) {
        HandleGameOver();
    }
}

void Game::HandleGameOver() {
    int finalScore = gameScore.GetScore();

    // Prüfe ob neuer Highscore erreicht wurde
    if (highscoreManager.IsNewHighscore(finalScore)) {
        // Bestimme Position in der Highscore-Liste
        int position = highscoreManager.GetHighscorePosition(finalScore);

        // Name Entry initialisieren
        stateManager.InitializeNameEntry(finalScore, position);
        stateManager.SetState(HIGHSCORE_ENTRY);
    }
    else {
        // Kein neuer Highscore - direkt zu Game Over
        stateManager.SetState(GAME_OVER);
    }
}

void Game::AddHighscoreEntry(const std::string& name, int score) {
    highscoreManager.AddHighscore(name, score);
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

    // Name Entry zurücksetzen
    stateManager.ResetNameEntry();
}