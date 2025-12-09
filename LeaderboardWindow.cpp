//
// Created by riley on 12/8/2025.
//

#include "LeaderboardWindow.hpp"

LeaderboardWindow::LeaderboardWindow(const sf::Font &font, const config &config) {
    width = config.colCount * 16;
    height = config.rowCount * 16 + 50;
    textWidth = static_cast<float>(width) / 2.0f;
    textHeight = static_cast<float>(height) / 2.0f;
    fileName = "files/leaderboard.txt";
    this->font = font;

    std::ifstream file(fileName);
    if (!file.is_open()) {std::cerr << "File not found" << std::endl;}
    std::string temp;
    unsigned int counter = 0;
    std::string timeNumber;
    std::string name;
    while (file >> temp){
        counter++;
        if (counter % 2 == 1 && counter > 1) {
            leaderboardList.insert(std::make_pair(std::make_pair(timeNumber.substr(0,2), timeNumber.substr(3, 2)), name));
        }
        counter % 2 == 1 ? timeNumber = temp : name = temp;

    }
    leaderboardList.insert(std::make_pair(std::make_pair(timeNumber.substr(0,2), timeNumber.substr(3, 2)), name));
    file.close();

    leaderboardText.setFont(font);
    leaderboardText.setString("LEADERBOARD");
    leaderboardText.setCharacterSize(20);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderboardText, textWidth, textHeight - 120.0f);

    leaderboardContents.setFont(font);
    leaderboardContents.setCharacterSize(18);
    leaderboardContents.setStyle(sf::Text::Bold);
    leaderboardContents.setFillColor(sf::Color::White);
    setText(leaderboardContents, textWidth, textHeight + 20.0f);
}


void LeaderboardWindow::Run(const PossibleOutcome &outcome, bool &inLeaderboard) {
    leaderboardWindow.create(sf::VideoMode(width, height), "Leaderboard Window");
    //game loop
    while (leaderboardWindow.isOpen()) {
        sf::Event event;
        //get the event
        while (leaderboardWindow.pollEvent(event)) {
            //if click the x in the corner, end without error
            if (event.type == sf::Event::Closed) {
                inLeaderboard = false;
                leaderboardWindow.close();
                return;
            }
        }

        readFromFile(outcome);


        leaderboardWindow.clear(sf::Color::Blue); //set window background blue
        leaderboardWindow.draw(leaderboardText);
        if (updatePending) {
            leaderboardContents.setString(records);
            setText(leaderboardContents, textWidth, textHeight + 20.0f);
            updatePending = false;
        }
        leaderboardWindow.draw(leaderboardContents);


        leaderboardWindow.display(); //display
    }
}


void LeaderboardWindow::setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void LeaderboardWindow::readFromFile(const PossibleOutcome &currentState) {
    if (!updatePending) return;

    std::ifstream file(fileName);


    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
    }

    char temp;
    records = "";
    while (file >> std::noskipws >> temp) {
        records += temp;
    }


    std::string desiredWord = "1.\t";
    std::string currentWord = "";
    size_t targetIndex = 0;
    size_t count = 0;
    unsigned int startingNumber = 1;
    for (char c: records) {
        if (c == ' ' || c == '\n' || c == ',') {
            currentWord = "";
        } else {
            currentWord += c;
            desiredWord += c;
        }
        if (c == ',') {
            desiredWord.append("\t");
        }
        if (c == '\n') {
            desiredWord += c;
            desiredWord.append("\n");
            startingNumber++;
            desiredWord.append(std::to_string(startingNumber));
            desiredWord.append(".\t");
        }
        if (currentWord == currentName) {
            targetIndex = count;
        }
        count++;
    }
    records = desiredWord;
    if (targetIndex != 0 && currentState == PossibleOutcome::win) {
        std::string firstHalf = desiredWord.substr(0, targetIndex + 3);
        std::string secondHalf = desiredWord.substr(targetIndex + 3);
        records = firstHalf + "*" + secondHalf;
    }


    file.close();
}

void LeaderboardWindow::writeToFile() {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        std::cout << "oh no!" << std::endl;
        std::cerr << "Error opening file!" << std::endl;
    }


    for (auto &record: leaderboardList) {
        if (leaderboardList.find(record) == (std::prev(leaderboardList.end()))) {
            file << record.first.first << ":" << record.first.second << ", " << record.second;
        }
        else {
            file << record.first.first << ":" << record.first.second << ", " << record.second << "\n";
        }
    }

    file.close();
}

void LeaderboardWindow::setCurrentName(const std::string &name) {
    currentName = name;
}

void LeaderboardWindow::addNewRecord(const std::pair<std::pair<std::string, std::string>, std::string> &newRecord) {
    //insert new record
    auto pair = leaderboardList.insert(newRecord);
    //if we have more than 5 records then get rid of the last one
    //since it is a set that is passed a functor that acts as comparator to ensure it is sorted from the lowest time to the highest time, the last element is the one that should be removed
    //I learned how to remove the last element of a set from:
    //https://en.cppreference.com/w/cpp/container/set/end.html
    //https://en.cppreference.com/w/cpp/iterator/prev.html
    if (leaderboardList.size() > 5) {
        leaderboardList.erase(std::prev(leaderboardList.end()));
    }

    //this is a little something i learned while studying for exam 3, and I was inspired because my technical interview question included adding elements into a set and seeing if they were inserted or not.
    if (pair.second) {
        setCurrentName(newRecord.second);
        writeToFile();
    }

    updatePending = true;
}
