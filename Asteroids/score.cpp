// score.cpp
#include "score.h"
#include "globals.h"
#include <fstream>
#include <algorithm>

// Definition der globalen Score-Variable
int g_currentScore = 0;

Score::Score() {
    currentScore = 0;
    highScore = 0;
    level = 1;
    LoadHighScore();
    g_currentScore = 0; // Globale Variable initialisieren
}

void Score::AddPoints(int points) {
    currentScore += points;
    highScore = std::max(highScore, currentScore);
    UpdateLevel();

    // Globale Variable aktualisieren für progressive Schwierigkeit
    g_currentScore = currentScore;
}

void Score::Reset() {
    currentScore = 0;
    level = 1;
    g_currentScore = 0; // Globale Variable zurücksetzen
}

void Score::UpdateLevel() {
    // Level bleibt für UI-Anzeige, aber wird nicht mehr für Schwierigkeit verwendet
    level = 1 + currentScore / 1000;
}

void Score::SaveHighScore() {
    std::ofstream file("highscore.dat");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

void Score::LoadHighScore() {
    std::ifstream file("highscore.dat");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}