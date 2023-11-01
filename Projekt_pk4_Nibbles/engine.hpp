#ifndef NIBBLES_01_ENGINE_HPP
#define NIBBLES_01_ENGINE_HPP

#include "snakesection.hpp"
#include "apple.hpp"
#include "superapple.hpp"
#include "wall.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <deque>
#include <fstream>
#include <ctime>
#include <filesystem>
#include <regex>
#include <ranges>
import score;

using namespace sf;
using namespace std;

class Engine {
private:
    // Okno
    Vector2f resolution;
    RenderWindow window;
    const unsigned int FPS = 60;
    static const Time TimePerFrame;

    vector<SnakeSection> snake;

    int snakeDirection;
    deque<int> directionQueue; // kolejka dla wciœniêæ klawiszy kierunkowych
    int speed;
    int sectionsToAdd; // ile sekcji dodaæ do wê¿a
    int applesEatenThisLevel;
    int applesEatenTotal;

    Apple apple;
    SuperApple superapple;


    vector<Wall> wallSections;
    int currentLevel;
    int maxLevels;
    vector<String> levels;

    Font mainFont;
    Text currentLevelText;
    Text scoreText;
    Text gameOverText;
    Text pressEnterText;

    Time timeSinceLastMove;

    int currentGameState;
    int lastGameState; // Do przechowywania ostatniego stanu gry podczas wstrzymywania.

public:
    enum Direction { UP, RIGHT, DOWN, LEFT };
    enum GameState { RUNNING, PAUSED, GAMEOVER };
    Engine();

    void input();
    void addDirection(int newDirection);
    void update();
    void draw();

    static void setupText(Text* textItem, const Font& font, const String& value, int size, Color colour);

    void newSnake();
    void addSnakeSection();

    void moveApple();
    void moveSuperApple();
    void checkLevelFiles();
    void loadLevel(int levelNumber);

    void beginNextLevel();
    void startTheGame();

    void togglePause();

    // G³ówna pêtla bêdzie w funkcji run
    void run();

};


#endif //NIBBLES_01_ENGINE_HPP
