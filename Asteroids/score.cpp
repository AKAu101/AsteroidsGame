// score.cpp
#include "score.h"
#include "globals.h"
#include <fstream>
#include <algorithm>

// Definition of the global score variable
int g_currentScore = 0;

/**
 * Constructor for Score class
 * Initializes score values to default starting state
 * Loads existing high score from file and synchronizes global score variable
 */
Score::Score() {
    currentScore = 0;
    highScore = 0;
    level = 1;
    LoadHighScore();
    g_currentScore = 0; // Initialize global variable
}

/**
 * Adds points to the current score and updates related systems
 * Updates high score if current score exceeds it
 * Recalculates level based on new score and synchronizes global variable
 * @param points Number of points to add to the current score
 */
void Score::AddPoints(int points) {
    currentScore += points;
    highScore = std::max(highScore, currentScore);
    UpdateLevel();

    // Update global variable for progressive difficulty
    g_currentScore = currentScore;
}

/**
 * Resets the score system to initial game state
 * Clears current score, resets level to 1, and synchronizes global variable
 * Used when starting a new game session
 */
void Score::Reset() {
    currentScore = 0;
    level = 1;
    g_currentScore = 0; // Reset global variable
}

/**
 * Updates the current level based on score progression
 * Level calculation remains for UI display but no longer affects difficulty
 * Progressive difficulty is now handled through the global score variable
 */
void Score::UpdateLevel() {
    // Level remains for UI display, but is no longer used for difficulty
    level = 1 + currentScore / 1000;
}

/**
 * Saves the current high score to persistent storage
 * Writes high score value to "highscore.dat" file
 * Ensures high score data persists between game sessions
 */
void Score::SaveHighScore() {
    std::ofstream file("highscore.dat");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

/**
 * Loads the high score from persistent storage
 * Reads high score value from "highscore.dat" file
 * Called during initialization to restore previous high score
 */
void Score::LoadHighScore() {
    std::ifstream file("highscore.dat");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}