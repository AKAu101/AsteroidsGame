// score.h - Score, Highscore
#ifndef SCORE_H
#define SCORE_H

class Score {
private:
    int currentScore;
    int highScore;
    int level;

public:
    Score();
    void AddPoints(int points);
    void Reset();
    int GetScore() const { return currentScore; }
    int GetHighScore() const { return highScore; }
    int GetLevel() const { return level; }
    void UpdateLevel();
    void SaveHighScore();
    void LoadHighScore();
};

#endif