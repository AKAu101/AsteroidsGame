#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>
#include <vector>

struct HighscoreEntry {
    std::string name = "";
    int score = 0;

    HighscoreEntry() = default;
    HighscoreEntry(const std::string& playerName, int playerScore)
        : name(playerName), score(playerScore) {
    }
};

class HighscoreManager {
private:
    std::vector<HighscoreEntry> highscores;
    static const int MAX_HIGHSCORES = 10;

    void LoadHighscores();
    void SaveHighscores();
    void SortHighscores();

public:
    HighscoreManager();
    ~HighscoreManager();

    // Hauptfunktionen
    void AddHighscore(const std::string& name, int score);
    bool IsNewHighscore(int score) const;
    int GetHighscorePosition(int score) const;

    // Getter
    const std::vector<HighscoreEntry>& GetHighscores() const { return highscores; }
    int GetLowestHighscore() const;
};

#endif