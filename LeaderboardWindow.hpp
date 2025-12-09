//
// Created by riley on 12/8/2025.
//

#ifndef MINESWEEPERPROJECT_LEADERBOARDWINDOW_HPP
#define MINESWEEPERPROJECT_LEADERBOARDWINDOW_HPP
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <set>
#include "config.hpp"
#include <iostream>
#include "PossibleOutcomes.hpp"
#include <iterator>

class LeaderboardWindow {
public:
    LeaderboardWindow(const sf::Font& font, const config& config);
    void Run(const PossibleOutcome& outcome, bool& inLeaderboard);
    void setText(sf::Text& text, float x, float y);
    void readFromFile(const PossibleOutcome& currentState); //read into records
    void writeToFile();//write to file
    void setCurrentName(const std::string& name);
    void addNewRecord(const std::pair<std::pair<std::string, std::string>, std::string>& newRecord);





private:
    //functor that will correctly sort the backing set of the leaderboard
    //I knew about functors and sets from this class, but I learned the syntax of passing a comparator to a set here:
    //https://en.cppreference.com/w/cpp/container/set/set.html
    struct CompareByTime {
        bool operator()(const std::pair<std::pair<std::string, std::string>, std::string>& lhs, const std::pair<std::pair<std::string, std::string>, std::string>& rhs) {
            int lhsTotalSeconds = std::stoi(lhs.first.first)*60 + std::stoi(lhs.first.second);
            int rhsTotalSeconds = std::stoi(rhs.first.first)*60 + std::stoi(rhs.first.second);
            if (lhsTotalSeconds != rhsTotalSeconds){return lhsTotalSeconds < rhsTotalSeconds;}
            return lhs.second < rhs.second;
        }
    };

    unsigned int width, height;
    float textWidth, textHeight;
    sf::RenderWindow leaderboardWindow;
    std::string records;
    std::string currentName;
    std::string fileName;
    sf::Text leaderboardText;
    sf::Text leaderboardContents;
    sf::Font font;
    std::set<std::pair<std::pair<std::string, std::string>, std::string>, CompareByTime> leaderboardList;
    bool updatePending = true;


};


#endif //MINESWEEPERPROJECT_LEADERBOARDWINDOW_HPP