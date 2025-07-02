
#include "Game.h"

#include <SFML/Graphics.hpp>

#include <iostream>

int main()
{
      // create window and set up
      sf::RenderWindow window(sf::VideoMode(1056, 720), 
          "Top-Down Adventure Game - More Games in C++");
      window.setFramerateLimit(60);

      //initialise an instance of the game class
      Game game(window);

      //run the init function of the game class and check it all initialises ok
      if (!game.init())
      {
            return 0;
      }

      // A Clock starts counting as soon as it's created
      sf::Clock clock;

      // Game loop: run the program as long as the window is open
      while (window.isOpen())
      {
            // check all the window's events that were triggered since the last
            // iteration of the loop
            sf::Event event;

            //calculate delta time
            sf::Time time = clock.restart();
            float dt = time.asSeconds();

            //'process inputs' element of the game loop
            while (window.pollEvent(event))
            {
                  // "close requested" event: we close the window
                  if (event.type == sf::Event::Closed)
                    window.close();

                  if (event.type == sf::Event::KeyPressed 
                      || event.type == sf::Event::KeyReleased)
                  {
                      game.keyInput(event);
                  }

                  if (event.type == sf::Event::MouseButtonPressed
                      || event.type == sf::Event::MouseButtonReleased)
                  {
                      game.mouseInput(event);
                  }
            }

            //'update' element of the game loop
            game.update(dt);

            window.clear(sf::Color::Black);

            //'render' element of the game loop
            game.render();
            window.display();
      }

      return 0;
}
