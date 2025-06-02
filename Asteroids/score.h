// score.h - Score, Highscore
#ifndef SCORE_H
#define SCORE_H

class Score {
private:
    int currentScore;       // Current game session score
    int highScore;          // Best score achieved across all sessions
    int level;              // Current level based on score progression

public:
    Score();

    void AddPoints(int points /* Number of points to add to current score */);
    void Reset();

    int GetScore() const {
        return currentScore;
    }
    int GetHighScore() const {
        return highScore;
    }
    int GetLevel() const {
        return level;
    }

    void UpdateLevel();
    void SaveHighScore();
    void LoadHighScore();
};

#endif