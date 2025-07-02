
#include "GameplayUI.h"

GameplayUI::GameplayUI()
{
    if (!font.loadFromFile("Data/Fonts/Prince.ttf"))
    {
        std::cout << "GameplayUI text font cannot not found!" << std::endl;
    }
    else
    {
        font.setSmooth(false);
        
        // Initialising the score UI with text
        if (!gem_icon_texture.loadFromFile(
            "Data/Graphics/UI/gem_ui_icon.png"))
        {
            std::cout << "GameplayUI gem icon texture cannot not found!" 
                << std::endl;
        }
        else
        {
            gem_icon.setTexture(gem_icon_texture);
            gem_icon.setPosition(64, 64);
            gem_icon.setScale(3, 3);
        }

        score_display.setFont(font);
        score_display.setCharacterSize(30);
        score_display.setPosition(96, 64);
        score_display.setFillColor(sf::Color::Black);
        score_display.setString("000000");

        // Initialising the health UI with individual heart icons
        if (!health_icon_texture.loadFromFile(
            "Data/Graphics/UI/heart_ui_icon.png"))
        {
            std::cout << "GameplayUI heart icon texture cannot not found!"
                << std::endl;
        }
        else
        {
            for (int i = 0; i < max_health_icons; i++)
            {
                std::shared_ptr<sf::Sprite> heart =
                    std::make_shared<sf::Sprite>(health_icon_texture);
                heart->setPosition(64 + (i * 36), 112);
                heart->setScale(3, 3);

                health_display.push_back(heart);
            }
        }
    }
}

void GameplayUI::initValues(int* health, int* score)
{
    if (health_ptr == nullptr)
    {
        health_ptr = std::shared_ptr<int>(health);
    }
    if (score_ptr == nullptr)
    {
        score_ptr = std::shared_ptr<int>(score);
    }
}

/*
    Update the current health the player has on screen
*/
void GameplayUI::updateHealth()
{
    if (health_ptr != nullptr)
    {
        for (int i = 0; i < max_health_icons; i++)
        {
            if (i < *health_ptr)
            {
                health_display[i]->setColor(full_colour);
            }
            else
            {
                health_display[i]->setColor(empty_colour);
            }
        }
    }
}

/*
    Update the current score the player has on screen
*/
void GameplayUI::updateScore()
{
    if (score_ptr != nullptr)
    {
        // Code for adding zeros to strings written by Ruslan Guseinov
        // Original code: https://stackoverflow.com/questions/6143824/add-leading-zeroes-to-string-without-sprintf
        std::string score_string = std::to_string(*score_ptr);
        std::string new_string = std::string(
            score_zfill - std::min(score_zfill, 
                score_string.length()), '0') 
            + score_string;
        score_display.setString(new_string);
    }
}

/*
    This should set both health and score displays back to default upon 
    restarting the game
*/
void GameplayUI::reset()
{
    updateHealth();
    updateScore();
}


