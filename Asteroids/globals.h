// globals.h - Korrigierte Version ohne Redefinitionen
#ifndef GLOBALS_H
#define GLOBALS_H

#include <algorithm>
#include <cmath>

// Bildschirm-Konstanten
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

// Spieler-Konstanten
#define STARTING_LIVES 3
#define SPACESHIP_ACCELERATION 300.0f
#define MAX_SPACESHIP_SPEED 200.0f
#define SPACESHIP_ROTATION_SPEED 180.0f

// Projektil-Konstanten
#define PROJECTILE_SPEED 400.0f
#define PROJECTILE_LIFETIME 3.0f

// Asteroid-Konstanten
#define ASTEROID_MIN_SPEED 50.0f
#define ASTEROID_MAX_SPEED 120.0f
#define LARGE_ASTEROID_SIZE 40
#define MEDIUM_ASTEROID_SIZE 25
#define SMALL_ASTEROID_SIZE 15
#define LARGE_ASTEROID_POINTS 20
#define MEDIUM_ASTEROID_POINTS 50
#define SMALL_ASTEROID_POINTS 100

// PowerUp-Konstanten
#define POWERUP_LIFETIME 15.0f

// Mathematische Konstanten (nur falls nicht bereits definiert)
#ifndef WINKEL2GRAD
#define WINKEL2GRAD 0.017453293f
#endif

// Globale Score-Variable für progressive Schwierigkeit
extern int g_currentScore;

// Hilfsfunktionen (verwende std:: Versionen um Konflikte zu vermeiden)
inline float SafeMax(float a, float b) { return std::max(a, b); }
inline float SafeMin(float a, float b) { return std::min(a, b); }

#endif