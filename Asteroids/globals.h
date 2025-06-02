// globals.h - Corrected version without redefinitions
#ifndef GLOBALS_H
#define GLOBALS_H

#include <algorithm>
#include <cmath>

// Screen constants
#define SCREEN_WIDTH 1200    // Game window width in pixels
#define SCREEN_HEIGHT 800    // Game window height in pixels

// Player constants
#define STARTING_LIVES 3                    // Initial number of lives at game start
#define SPACESHIP_ACCELERATION 300.0f      // Thrust acceleration in pixels per second squared
#define MAX_SPACESHIP_SPEED 200.0f         // Maximum movement speed in pixels per second
#define SPACESHIP_ROTATION_SPEED 180.0f    // Rotation speed in degrees per second

// Projectile constants
#define PROJECTILE_SPEED 400.0f     // Projectile movement speed in pixels per second
#define PROJECTILE_LIFETIME 3.0f    // Time in seconds before projectile expires

// Asteroid constants
#define ASTEROID_MIN_SPEED 50.0f    // Minimum asteroid movement speed in pixels per second
#define ASTEROID_MAX_SPEED 120.0f   // Maximum asteroid movement speed in pixels per second
#define LARGE_ASTEROID_SIZE 40      // Radius in pixels for large asteroids
#define MEDIUM_ASTEROID_SIZE 25     // Radius in pixels for medium asteroids
#define SMALL_ASTEROID_SIZE 15      // Radius in pixels for small asteroids
#define LARGE_ASTEROID_POINTS 20    // Points awarded for destroying large asteroids
#define MEDIUM_ASTEROID_POINTS 50   // Points awarded for destroying medium asteroids
#define SMALL_ASTEROID_POINTS 100   // Points awarded for destroying small asteroids

// PowerUp constants
#define POWERUP_LIFETIME 15.0f      // Time in seconds before power-up expires
#define POWERUP_SPAWN_INTERVAL 15.0f // Time in seconds between automatic power-up spawns

// Mathematical constants (only if not already defined)
#ifndef WINKEL2GRAD
#define WINKEL2GRAD 0.017453293f    // Conversion factor from degrees to radians (π/180)
#endif

// Global score variable for progressive difficulty
extern int g_currentScore;          // Current game score used for difficulty scaling

// Helper functions (use std:: versions to avoid conflicts)
inline float SafeMax(float a /* First value to compare */,
    float b /* Second value to compare */) {
    return std::max(a, b);
}

inline float SafeMin(float a /* First value to compare */,
    float b /* Second value to compare */) {
    return std::min(a, b);
}

#endif