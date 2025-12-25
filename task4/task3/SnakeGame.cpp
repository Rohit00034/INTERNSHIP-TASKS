#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <optional>
#include <algorithm>

using namespace sf;
using namespace std;

struct SnakePart {
    int x;
    int y;
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    const int screenWidth = 800;
    const int screenHeight = 600;
    const int gridSize = 20;

    int score = 0;
    int highScore = 0;
    bool gameOver = false;
    bool paused = false;

    RenderWindow window(
        VideoMode(Vector2u(screenWidth, screenHeight)),
        "Snake Game"
    );
    window.setFramerateLimit(10);

    Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        return -1; // fail safely if font not found
    }

    vector<SnakePart> snake;
    SnakePart head{
        (screenWidth / 2 / gridSize) * gridSize,
        (screenHeight / 2 / gridSize) * gridSize
    };
    snake.push_back(head);

    int dirX = gridSize;
    int dirY = 0;

    int foodX = (rand() % (screenWidth / gridSize)) * gridSize;
    int foodY = (rand() % (screenHeight / gridSize)) * gridSize;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPress = event->getIf<Event::KeyPressed>()) {
                auto key = keyPress->code;

                if (key == Keyboard::Key::Up && dirY == 0) {
                    dirX = 0; dirY = -gridSize;
                } else if (key == Keyboard::Key::Down && dirY == 0) {
                    dirX = 0; dirY = gridSize;
                } else if (key == Keyboard::Key::Left && dirX == 0) {
                    dirX = -gridSize; dirY = 0;
                } else if (key == Keyboard::Key::Right && dirX == 0) {
                    dirX = gridSize; dirY = 0;
                } else if (key == Keyboard::Key::Space) {
                    paused = !paused;
                } else if (key == Keyboard::Key::R && gameOver) {
                    snake.clear();
                    snake.push_back(head);
                    dirX = gridSize;
                    dirY = 0;
                    score = 0;
                    gameOver = false;
                    foodX = (rand() % (screenWidth / gridSize)) * gridSize;
                    foodY = (rand() % (screenHeight / gridSize)) * gridSize;
                }
            }
        }

        if (!gameOver && !paused) {
            SnakePart newHead{ snake[0].x + dirX, snake[0].y + dirY };

            if (newHead.x < 0 || newHead.x >= screenWidth ||
                newHead.y < 0 || newHead.y >= screenHeight) {
                gameOver = true;
                highScore = max(highScore, score);
            }

            for (const auto& part : snake) {
                if (newHead.x == part.x && newHead.y == part.y) {
                    gameOver = true;
                    highScore = max(highScore, score);
                    break;
                }
            }

            if (!gameOver) {
                snake.insert(snake.begin(), newHead);

                if (newHead.x == foodX && newHead.y == foodY) {
                    score += 10;
                    foodX = (rand() % (screenWidth / gridSize)) * gridSize;
                    foodY = (rand() % (screenHeight / gridSize)) * gridSize;
                } else {
                    snake.pop_back();
                }
            }
        }

        window.clear(Color(20, 20, 40));

        for (size_t i = 0; i < snake.size(); ++i) {
            RectangleShape part(Vector2f(gridSize - 2.f, gridSize - 2.f));
            part.setPosition(Vector2f(
                float(snake[i].x + 1),
                float(snake[i].y + 1)
            ));
            part.setFillColor(i == 0 ? Color::Green : Color(0, 200, 80));
            window.draw(part);
        }

        CircleShape food(gridSize / 2.f - 1.f);
        food.setPosition(Vector2f(
            float(foodX + 1),
            float(foodY + 1)
        ));
        food.setFillColor(Color::Red);
        window.draw(food);

        Text scoreText(font, "Score: " + to_string(score), 24);
        scoreText.setPosition(Vector2f(10.f, 10.f));
        window.draw(scoreText);

        Text highScoreText(font, "High: " + to_string(highScore), 24);
        highScoreText.setPosition(Vector2f(
            float(screenWidth - 140),
            10.f
        ));
        window.draw(highScoreText);

        if (paused) {
            Text pausedText(font, "PAUSED", 50);
            pausedText.setPosition(Vector2f(
                float(screenWidth / 2 - 100),
                float(screenHeight / 2 - 40)
            ));
            window.draw(pausedText);
        }

        if (gameOver) {
            Text gameOverText(font, "GAME OVER", 50);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setPosition(Vector2f(
                float(screenWidth / 2 - 140),
                float(screenHeight / 2 - 40)
            ));
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
