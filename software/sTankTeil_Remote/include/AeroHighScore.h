#pragma once
#include <string>

class AeroHighScore {
public:
    // Default constructor added from the src version
    AeroHighScore() : score(0), level(0), lines(0), name("") {}
    
    // Existing constructor
    AeroHighScore(int score, int level, int lines, const std::string& name) :
        score(score), level(level), lines(lines), name(name) {}
    
    // Getters
    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getLines() const { return lines; }
    std::string getName() const { return name; }
    
    // Comparison operator for sorting (higher scores first)
    bool operator<(const AeroHighScore& other) const {
        return score > other.score;
    }

private:
    int score;
    int level;
    int lines;
    std::string name;
};
