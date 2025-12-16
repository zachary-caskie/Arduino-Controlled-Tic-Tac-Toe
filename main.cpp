#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>
#include "SerialPort.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080}), "Tic tac Toe", sf::State::Fullscreen);

    sf::RectangleShape filter({ 1920, 1080 });
    filter.setFillColor(sf::Color(34, 34, 34, 100));

    sf::Font winFont;
    if (!winFont.openFromFile("assets/uiFont.ttf")) {
        std::cerr << "Failed to load ui font\n";
    }
    sf::Text winText(winFont);
    winText.setString("Player #" + std::to_string(getWinner()) + " Wins!");
    winText.setFillColor(sf::Color::White);
    winText.setOutlineColor(sf::Color::Black);
    winText.setCharacterSize(40);
    winText.setOrigin({ winText.getLocalBounds().size.x / 2, winText.getLocalBounds().size.y / 2 });
    winText.setPosition({ 1920/2, 1080/2 - 20});

    sf::Text infoText(winFont);
    infoText.setString("Click to restart");
    infoText.setFillColor(sf::Color::White);
    infoText.setOutlineColor(sf::Color::Black);
    infoText.setCharacterSize(40);
    infoText.setOrigin({ infoText.getLocalBounds().size.x / 2, infoText.getLocalBounds().size.y / 2 });
    infoText.setPosition({ 1920/2, 1080/2 + 10});


    startGame();

    SerialPort arduino("COM8", CBR_9600);

    sf::Vector2i dir;
    bool press = false;;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        //------UPDATE-----
        //Parser controller data
          //---------Parser------------
        std::string line;
        if (arduino.readLine(line)) {

            std::cout << "RAW: [" << line << "]\n";

            std::string xs, ys, ps;
            std::stringstream ss(line);

            if (std::getline(ss, xs, '/') &&
                std::getline(ss, ys, '/') &&
                std::getline(ss, ps, '/')
                )
            {
                float x = std::stof(xs);
                float y = std::stof(ys);
                press = (ps == "1");

                //normalize joystick values
                float nx = -((x - 512.0f) / 512.0f);
                float ny = -((y - 512.0f) / 512.0f);

                const float deadzone = 0.05f;

                if (std::abs(nx) < deadzone) nx = 0;
                if (std::abs(ny) < deadzone) ny = 0;

                if (x < 490) { dir.x = 1; }
                else if (x > 500) { dir.x = -1; }
                else { dir.x = 0; }
                if (y > 501) { dir.y = -1; }
                else if (y < 501) { dir.y = 1; }
                else { dir.y = 0; }

            }
        }
        //Game loop

        update(dir, press);
        winText.setString("Player #" + std::to_string(getWinner()) + " Wins!");
        if (getWinner() == 0)
        {
            winText.setString("Draw");
        }
        winText.setOrigin({ winText.getLocalBounds().size.x / 2, winText.getLocalBounds().size.y / 2 });
        winText.setPosition({ 1920 / 2, 1080 / 2 - 20 });

        if (interact(press) && isWon())
        {
            startGame();
        }

        //-------DRAW-------
        window.clear();
        drawGame(window);
        if (isWon())
        {
            window.draw(filter);
            window.draw(winText);
            window.draw(infoText);
        }
        window.display();
    }
}