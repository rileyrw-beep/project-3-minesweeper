//
// Created by riley on 12/1/2025.
//

#include "WelcomeWindow.hpp"


WelcomeWindow::WelcomeWindow(const sf::Font& font, const config& config) {

    //set the width and height for the welcome window based off the row and col count found in config
    width = config.colCount*32;
    height = config.rowCount*32+100;

    //store the text's position width and height based on the window's
    textWidth = static_cast<float>(width) / 2.0f;
    textHeight = static_cast<float>(height) / 2.0f;

    userString = "";

    //set welcome text
    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, textWidth, textHeight - 150.0f);

    //set up the enter name text
    enterNameText.setFont(font);
    enterNameText.setString("Enter your name:");
    enterNameText.setCharacterSize(20);
    enterNameText.setFillColor(sf::Color::White);
    enterNameText.setStyle(sf::Text::Bold);
    setText(enterNameText, textWidth, textHeight - 75.0f);

    //set up the text where the user will enter the name
    userText.setFont(font);
    userText.setString(userString);
    userText.setCharacterSize(18);
    userText.setFillColor(sf::Color::Yellow);
    userText.setStyle(sf::Text::Bold);
    setText(userText, textWidth, textHeight - 45.0f);

    //make the cursor thingy that follows where you type
    cursor.setSize({2, 24});
    cursor.setFillColor(sf::Color::Yellow);
    cursor.setPosition(userText.getGlobalBounds().left + userText.getLocalBounds().width, userText.getPosition().y);

    //create said welcome window based on the width and height
    welcomeWindow.create(sf::VideoMode(width, height), "Welcome Window");
}

bool WelcomeWindow::Run(const config& config) {

    //game loop
    while (welcomeWindow.isOpen()) {
        sf::Event event;
        //get the event
        while (welcomeWindow.pollEvent(event)) {
            //if click the x in the corner, end without error
            if (event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return true;
            }
            // if its a key pressed then check what key it was
            if (event.type == sf::Event::KeyPressed) {
                //if its a backspace, delete the last letter typed assuming there is a letter there, and move the cursor
                if (event.key.code == sf::Keyboard::BackSpace && userString.size() > 0) {
                    userString.pop_back();
                    setDynamicText(userText, textWidth, textHeight-45.0f);
                    cursor.setPosition(userText.getGlobalBounds().left + userText.getLocalBounds().width-8.f, userText.getPosition().y);
                }
                else if (event.key.code == sf::Keyboard::Enter && userString.size() > 0) {
                    welcomeWindow.close();
                }

            }
            //if it is text entered, append it to the user inputted text as long it is
                //1: a viable letter
                //2: not longer than 10 letters
                    //with the caveat that the first letter is forced uppercase everything else forced lower case
            //and then at the end move the cursor so that it fits
            else if (event.type == sf::Event::TextEntered)
            {
                if (((event.text.unicode >= 65  && event.text.unicode <= 90) || (event.text.unicode >= 97  && event.text.unicode <= 122)) && userString.size() < 10 && event.text.unicode != 8) {
                    char c = static_cast<char>(event.text.unicode);
                    if (userString.size() == 0) {
                        c = std::toupper(c);
                    }
                    else {
                        c = std::tolower(c);
                    }
                    userString.push_back(c);
                    setDynamicText(userText, textWidth, textHeight-45.0f);
                    cursor.setPosition(userText.getGlobalBounds().left + userText.getLocalBounds().width+16.f, userText.getPosition().y);
                }


            }
        }

        welcomeWindow.clear(sf::Color::Blue); //set window background blue
        welcomeWindow.draw(welcomeText); //draw welcome text
        welcomeWindow.draw(enterNameText); //draw the text that says enter yer name
        userText.setString(userString); //set what the user inputted text is to whatever new thing the user inputted
        welcomeWindow.draw(userText); //draw the thing above
        welcomeWindow.draw(cursor); //redraw the cursor


        welcomeWindow.display(); //display
    }
    return false;
}

void WelcomeWindow::setText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void WelcomeWindow::setDynamicText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, text.getOrigin().y);
    text.setPosition(sf::Vector2f(x, y));
}

std::string WelcomeWindow::getName() const{
    return userString;
}


