#include <SFML/Graphics.hpp>
#include <optional>
#include "NumberSlideGame.hpp"
#include<string>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({640, 720}),
        "Sliding Number Puzzle");

    Matrix board;
    int moveCount = 0;
    AstarSolver solver;
    std::string hintMessage = "Press H for a hint";

    std::vector<Direction> solutionMoves;
    int solutionIndex = 0;
    bool autoSolving = false;
    sf::Clock solveClock;

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        return 1;
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (autoSolving && key->code != sf::Keyboard::Key::R)
                continue;

                if (key->code == sf::Keyboard::Key::Left ||
                    key->code == sf::Keyboard::Key::A)
                {
                    if (board.Update_Matrix(Direction::Left))
                        ++moveCount;
                }
                else if (key->code == sf::Keyboard::Key::Right ||
                        key->code == sf::Keyboard::Key::D)
                {
                    if (board.Update_Matrix(Direction::Right))
                        ++moveCount;
                }
                else if (key->code == sf::Keyboard::Key::Up ||
                        key->code == sf::Keyboard::Key::W)
                {
                    if (board.Update_Matrix(Direction::Up))
                        ++moveCount;
                }
                else if (key->code == sf::Keyboard::Key::Down ||
                        key->code == sf::Keyboard::Key::S)
                {
                    if (board.Update_Matrix(Direction::Down))
                        ++moveCount;
                }

                else if (key->code == sf::Keyboard::Key::H)
                {
                    const auto solution = solver.solve(board.getState());

                    if (solution.empty())
                    {
                        hintMessage = board.won()
                            ? "Puzzle already solved!"
                            : "No solution found";
                    }
                    else
                    {
                        switch (solution.front())
                        {
                            case Direction::Left:  hintMessage = "Hint: press A or Left";  break;
                            case Direction::Right: hintMessage = "Hint: press D or Right"; break;
                            case Direction::Up:    hintMessage = "Hint: press W or Up";    break;
                            case Direction::Down:  hintMessage = "Hint: press S or Down";  break;
                        }
                    }
                }
                else if (key->code == sf::Keyboard::Key::R)
                {
                    board = Matrix();
                    moveCount = 0;

                    autoSolving = false;
                    solutionMoves.clear();
                    solutionIndex = 0;

                    hintMessage = "Press H for a hint";
                }
                else if (key->code == sf::Keyboard::Key::Enter)
                {
                    solutionMoves = solver.solve(board.getState());
                    solutionIndex = 0;

                    if (solutionMoves.empty())
                    {
                        autoSolving = false;
                        hintMessage = board.won()
                            ? "Puzzle already solved!"
                            : "No solution found";
                    }
                    else
                    {
                        autoSolving = true;
                        solveClock.restart();
                        hintMessage = "Showing solution...";
                    }
                }
            }
        }

        

        if (autoSolving &&
                    solveClock.getElapsedTime().asMilliseconds() >= 200)
        {
            board.Update_Matrix(solutionMoves[solutionIndex]);
            ++moveCount;
            ++solutionIndex;
            solveClock.restart();

            if (solutionIndex == static_cast<int>(solutionMoves.size()))
            {
                autoSolving = false;
                hintMessage = "Puzzle solved!";
            }
        }





        if (board.won())    hintMessage = "Puzzle solved!"
        ;
        window.clear(sf::Color(25, 29, 39));
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                const int value = board.getTile(row, col);
                const float x = 85.f + col * 120.f;
                const float y = 125.f + row * 120.f;

                sf::RectangleShape tile({110.f, 110.f});
                tile.setPosition({x, y});
                tile.setFillColor(
                    value == 0 ? sf::Color(25, 29, 39) : sf::Color(60, 70, 90));
                tile.setOutlineThickness(2.f);
                tile.setOutlineColor(sf::Color(100, 120, 150));
                window.draw(tile);

                if (value != 0)
                {
                    sf::Text number(font, std::to_string(value), 40);
                    number.setFillColor(sf::Color::White);

                    const sf::FloatRect bounds = number.getLocalBounds();
                    number.setOrigin({
                        bounds.position.x + bounds.size.x / 2.f,
                        bounds.position.y + bounds.size.y / 2.f
                    });
                    number.setPosition({x + 55.f, y + 55.f});

                    window.draw(number);
                }
            }
        }

        sf::Text movesText(font, "Moves: " + std::to_string(moveCount), 24);
        movesText.setPosition({250.f, 45.f});
        movesText.setFillColor(sf::Color::White);
        window.draw(movesText);
        sf::Text hintText(font, hintMessage, 20);
        hintText.setPosition({190.f, 85.f});
        hintText.setFillColor(sf::Color::White);
        window.draw(hintText);

        sf::Text controlsText(
                        font,
                        "Arrows/W A S D :- move   H :- hint\n\nEnter :- solve       R :- restart",
                        16);
        controlsText.setPosition({85.f, 605.f});
        controlsText.setFillColor(sf::Color(190, 200, 215));
        window.draw(controlsText);

        window.display();
    }
}