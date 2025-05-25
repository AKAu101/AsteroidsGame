// score.cpp
#include "score.h"
#include <fstream>
#include <algorithm>

Score::Score() {
    currentScore = 0;
    highScore = 0;
    level = 1;
    LoadHighScore();
}

void Score::AddPoints(int points) {
    currentScore += points;
    highScore = std::max(highScore, currentScore);
    UpdateLevel();
}

void Score::Reset() {
    currentScore = 0;
    level = 1;
}

void Score::UpdateLevel() {
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