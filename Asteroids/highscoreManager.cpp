#include "highscoreManager.h"
#include <fstream>
#include <algorithm>

/**
 * Constructor for HighscoreManager class
 * Automatically loads existing highscores from file on creation
 * If no file exists, default highscores will be created
 */
HighscoreManager::HighscoreManager() {
    LoadHighscores();
}

/**
 * Destructor for HighscoreManager class
 * Automatically saves all highscores to file before destruction
 * Ensures no highscore data is lost when the object is destroyed
 */
HighscoreManager::~HighscoreManager() {
    SaveHighscores();
}

/**
 * Loads highscores from the data file into memory
 * Reads name-score pairs from "highscores.dat" file
 * Creates default highscores if file is empty or doesn't exist
 * Automatically sorts highscores after loading
 */
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

    // Add default highscores if file is empty or doesn't exist
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

/**
 * Saves all current highscores to the data file
 * Writes name-score pairs to "highscores.dat" in plain text format
 * Each entry is written on a separate line with space separation
 */
void HighscoreManager::SaveHighscores() {
    std::ofstream file("highscores.dat");
    if (file.is_open()) {
        for (const auto& entry : highscores) {
            file << entry.name << " " << entry.score << "\n";
        }
        file.close();
    }
}

/**
 * Sorts highscores in descending order (highest score first)
 * Uses lambda function for custom comparison based on score values
 * Trims the list to MAX_HIGHSCORES if it exceeds the limit
 */
void HighscoreManager::SortHighscores() {
    std::sort(highscores.begin(), highscores.end(),
        [](const HighscoreEntry& a, const HighscoreEntry& b) {
            return a.score > b.score;
        });

    if (highscores.size() > MAX_HIGHSCORES) {
        highscores.resize(MAX_HIGHSCORES);
    }
}

/**
 * Checks if a given score qualifies as a new highscore
 * Returns true if the highscore list isn't full or if the score beats the lowest entry
 * @param score The score to check against current highscores
 * @return True if the score qualifies for the highscore list
 */
bool HighscoreManager::IsNewHighscore(int score) const {
    return highscores.size() < MAX_HIGHSCORES ||
        score > GetLowestHighscore();
}

/**
 * Adds a new highscore entry to the list
 * Automatically sorts the list and saves to file after adding
 * The new entry will be inserted in the correct position based on score
 * @param name Player's name for the highscore entry
 * @param score Player's achieved score
 */
void HighscoreManager::AddHighscore(const std::string& name, int score) {
    highscores.emplace_back(name, score);
    SortHighscores();
    SaveHighscores();
}

/**
 * Determines the position where a new score would be inserted
 * Scans through sorted highscores to find the correct ranking position
 * Used for displaying achievement rank to the player
 * @param score The score to find the position for
 * @return Integer position (0-based) where the score would rank
 */
int HighscoreManager::GetHighscorePosition(int score) const {
    for (size_t i = 0; i < highscores.size(); i++) {
        if (score > highscores[i].score) {
            return (int)i;
        }
    }
    return (int)highscores.size();
}

/**
 * Returns the lowest score in the current highscore list
 * Used to determine if a new score qualifies for the highscore table
 * Returns 0 if the highscore list is empty
 * @return Integer value of the lowest highscore, or 0 if list is empty
 */
int HighscoreManager::GetLowestHighscore() const {
    if (highscores.empty()) return 0;
    return highscores.back().score;
}