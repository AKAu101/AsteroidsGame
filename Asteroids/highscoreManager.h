#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>
#include <vector>

struct HighscoreEntry {
    std::string name = "";      // Player's name (max 5 characters)
    int score = 0;              // Achieved score value

    HighscoreEntry() = default;

    HighscoreEntry(const std::string& playerName /* Player's name for the entry */,
        int playerScore /* Score value achieved by the player */)
        : name(playerName), score(playerScore) {
    }
};

class HighscoreManager {
private:
    std::vector<HighscoreEntry> highscores;     // Collection of high score entries
    static const int MAX_HIGHSCORES = 10;      // Maximum number of high scores to store

    void LoadHighscores();
    void SaveHighscores();
    void SortHighscores();

public:
    HighscoreManager();
    ~HighscoreManager();

    // Main functions
    void AddHighscore(const std::string& name /* Player's name (max 5 characters) */,
        int score /* Score value to add to the list */);
    bool IsNewHighscore(int score /* Score to check against current high scores */) const;
    int GetHighscorePosition(int score /* Score to find ranking position for */) const;

    // Getter
    const std::vector<HighscoreEntry>& GetHighscores() const { return highscores; }
    int GetLowestHighscore() const;
};

#endif