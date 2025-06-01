#include "highscoreManager.h"
#include <fstream>
#include <algorithm>

HighscoreManager::HighscoreManager() {
    LoadHighscores();
}

HighscoreManager::~HighscoreManager() {
    SaveHighscores();
}

void HighscoreManager::LoadHighscores() {
    highscores.clear();
    std::ifstream file("highscores.dat");

    if (file.is_open()) {
        std::string name;
        int score;

        while (file >> name >> score && highscores.size() < MAX_HIGHSCORES) {
            highscores.emplace_back(name, score);
        }
        file.close();
    }

    // Standard-Highscores hinzufügen wenn Datei leer oder nicht vorhanden
    if (highscores.empty()) {
        highscores.emplace_back("ADMIN", 10000);
        highscores.emplace_back("PILOT", 8500);
        highscores.emplace_back("JUU__", 7000);
        highscores.emplace_back("NOVA_", 5500);
        highscores.emplace_back("STAR_", 4000);
        highscores.emplace_back("COMET", 3000);
        highscores.emplace_back("ORBIT", 2000);
        highscores.emplace_back("MOON_", 1500);
        highscores.emplace_back("SOLAR", 1000);
        highscores.emplace_back("ASTRO", 500);
    }

    SortHighscores();
}

void HighscoreManager::SaveHighscores() {
    std::ofstream file("highscores.dat");

    if (file.is_open()) {
        for (const auto& entry : highscores) {
            file << entry.name << " " << entry.score << "\n";
        }
        file.close();
    }
}

void HighscoreManager::SortHighscores() {
    std::sort(highscores.begin(), highscores.end(),
        [](const HighscoreEntry& a, const HighscoreEntry& b) {
            return a.score > b.score;
        });

    if (highscores.size() > MAX_HIGHSCORES) {
        highscores.resize(MAX_HIGHSCORES);
    }
}

bool HighscoreManager::IsNewHighscore(int score) const {
    return highscores.size() < MAX_HIGHSCORES ||
        score > GetLowestHighscore();
}

void HighscoreManager::AddHighscore(const std::string& name, int score) {
    highscores.emplace_back(name, score);
    SortHighscores();
    SaveHighscores();
}

int HighscoreManager::GetHighscorePosition(int score) const {
    for (size_t i = 0; i < highscores.size(); i++) {
        if (score > highscores[i].score) {
            return (int)i;
        }
    }
    return (int)highscores.size();
}

int HighscoreManager::GetLowestHighscore() const {
    if (highscores.empty()) return 0;
    return highscores.back().score;
}