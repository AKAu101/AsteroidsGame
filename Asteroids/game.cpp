#include "game.h"
#include "globals.h"
#include "highscoreManager.h"
#include <iostream>

/**
 * Constructor for the Game class
 * Initializes all game components, systems, and dependencies
 * Sets up audio system and connects UI renderer with highscore manager
 */
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
    uiRenderer.SetHighscoreManager(&highscoreManager);
}

/**
 * Destructor for the Game class
 * Saves the high score and unloads all audio resources
 */
Game::~Game() {
    gameScore.SaveHighScore();
    UnloadGameSounds();
}

/**
 * Initializes the game window and core game settings
 * Sets up the game window, target framerate, and spawns initial asteroids
 */
void Game::Initialize() {
    gameRunning = true;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids");
    SetTargetFPS(60);
    objectManager.SpawnAsteroids(4);
}

/**
 * Main game update loop
 * Updates background music, handles input, and updates game state
 * Calls specific update methods based on current game state
 */
void Game::Update() {
    float deltaTime = GetFrameTime();

    // Update background music
    UpdateMusicStream(backgroundMusic);

    inputHandler.HandleInput(deltaTime);
    stateManager.Update(deltaTime);

    if (stateManager.GetCurrentState() == IN_GAME) {
        UpdateInGame(deltaTime);
    }
}

/**
 * Updates all in-game logic during active gameplay
 * Handles player updates, object management, collision detection, and spawning
 */
void Game::UpdateInGame(float deltaTime) {
    player.Update(deltaTime);
    objectManager.UpdateObjects(deltaTime);
    UpdateTimers(deltaTime);

    bool isInvulnerable = player.IsInvulnerable();

    collisionSystem.CheckCollisions(hasShield, isInvulnerable);
    HandleSpawning(deltaTime);
    CheckGameState();
    CheckPowerUpCollisions();
}

/**
 * Checks for collisions between the player and power-ups
 * Handles power-up collection and applies their effects
 */
void Game::CheckPowerUpCollisions() {
    Rectangle playerBounds = player.GetBounds();

       // Check all power-ups
    auto& powerups = const_cast<std::vector<PowerUp>&>(objectManager.GetPowerUps());

    for (auto& powerup : powerups) {
        if (powerup.IsActive()) {
            Rectangle powerupBounds = powerup.GetBounds();

            if (CheckCollisionRecs(playerBounds, powerupBounds)) {
                printf("COLLISION DETECTED! PowerUp Type: %d\n", (int)powerup.GetType());

                // Collect power-up
                PowerUpType type = powerup.GetType();
                powerup.Collect(); // Deactivate power-up

                // Set item based on type
                switch (type) {
                case EXTRA_LIFE:
                    printf("Collected EXTRA_LIFE - adding life immediately\n");
                    player.AddLife();
                    currentItem = 0; // Use immediately, don't store
                    break;

                case RAPID_FIRE:
                    printf("Collected RAPID_FIRE - stored as item\n");
                    currentItem = 1; // Store as item
                    break;

                case SHIELD:
                    printf("Collected SHIELD - stored as item\n");
                    currentItem = 2; // Store as item
                    break;

                default:
                    printf("Unknown PowerUp type: %d\n", (int)type);
                    break;
                }

                printf("currentItem is now: %d\n", currentItem);
                break; // Only collect one power-up per frame
            }
        }
    }
}

/**
 * Main drawing method for the game
 * Begins drawing context, renders current game state, and ends drawing
 */
void Game::Draw() {
    BeginDrawing();
    uiRenderer.DrawCurrentState();
    EndDrawing();
}

/**
 * Initializes all game audio systems and loads sound files
 * Sets up audio device, loads sound effects and background music
 * Configures volume levels and starts background music playback
 */
void Game::InitGameSounds() {
    InitAudioDevice();
    shootSound = LoadSound("Sounds/shoot.wav");
    SetSoundVolume(shootSound, 0.3f);

    backgroundMusic = LoadMusicStream("Sounds/Soundtrack.ogg");
    SetMusicVolume(backgroundMusic, 0.1f);
    PlayMusicStream(backgroundMusic);
}

/**
 * Unloads all audio resources and closes the audio device
 * Properly cleans up sound effects, music streams, and audio system
 */
void Game::UnloadGameSounds() {
    UnloadSound(shootSound);
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
}

/**
 * Plays the shooting sound effect
 * Checks if sound is properly loaded before attempting to play
 */
void Game::PlayShootSound() {
    if (shootSound.frameCount > 0) {
        PlaySound(shootSound);
    }
}

/**
 * Updates all game timers
 * Decrements projectile cooldown and increments asteroid spawn timer
 */
void Game::UpdateTimers(float deltaTime) {
    if (projectileCooldown > 0) projectileCooldown -= deltaTime;
    asteroidSpawnTimer += deltaTime;
}

/**
 * Handles asteroid spawning logic based on game progression
 * Calculates dynamic spawn timing based on current score
 * Spawns new asteroids at random edge positions when timer expires
 */
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

/**
 * Checks current game state and handles level progression
 * Spawns new asteroid waves when all asteroids are destroyed
 * Triggers game over sequence when player runs out of lives
 */
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

/**
 * Handles game over logic and highscore checking
 * Determines if player achieved a new highscore and transitions to appropriate state
 * Either enters name entry for new highscore or goes directly to game over screen
 */
void Game::HandleGameOver() {
    int finalScore = gameScore.GetScore();

    // Check if new highscore was achieved
    if (highscoreManager.IsNewHighscore(finalScore)) {
        // Determine position in highscore list
        int position = highscoreManager.GetHighscorePosition(finalScore);

        // Initialize name entry
        stateManager.InitializeNameEntry(finalScore, position);
        stateManager.SetState(HIGHSCORE_ENTRY);
    }
    else {
        // No new highscore - go directly to game over
        stateManager.SetState(GAME_OVER);
    }
}

/**
 * Adds a new highscore entry to the highscore manager
 * Called when player enters their name for a new highscore
 */
void Game::AddHighscoreEntry(const std::string& name, int score) {
    highscoreManager.AddHighscore(name, score);
}

/**
 * Resets all game state to initial conditions for a new game
 * Resets score, player state, objects, timers, and power-up states
 * Spawns initial asteroid wave and resets name entry system
 */
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

    // Reset name entry
    stateManager.ResetNameEntry();
}