#ifndef GLOBALS_H
#define GLOBALS_H

// globals.h - Gemeinsame Konstanten
static constexpr int SCREEN_WIDTH = 1200;
static constexpr int SCREEN_HEIGHT = 800;
static constexpr int STARTING_LIVES = 3;
static constexpr float MAX_SPACESHIP_SPEED = 400.0f;
static constexpr float SPACESHIP_ACCELERATION = 300.0f;
static constexpr float SPACESHIP_ROTATION_SPEED = 200.0f;
static constexpr float PROJECTILE_SPEED = 500.0f;
static constexpr float PROJECTILE_LIFETIME = 2.0f;
static constexpr int MAX_PROJECTILES = 10;
static constexpr int MAX_ASTEROIDS = 20;
static constexpr float ASTEROID_MIN_SPEED = 50.0f;
static constexpr float ASTEROID_MAX_SPEED = 150.0f;
static constexpr int LARGE_ASTEROID_SIZE = 50;
static constexpr int MEDIUM_ASTEROID_SIZE = 30;
static constexpr int SMALL_ASTEROID_SIZE = 15;
static constexpr int LARGE_ASTEROID_POINTS = 20;
static constexpr int MEDIUM_ASTEROID_POINTS = 50;
static constexpr int SMALL_ASTEROID_POINTS = 100;
static constexpr float POWERUP_SPAWN_RATE = 0.3f;
static constexpr float POWERUP_LIFETIME = 10.0f;
static constexpr int MAX_POWERUPS = 10;

static constexpr float WINKEL2GRAD = 0.01745329252f; // Degree to Radian conversion factor


#endif