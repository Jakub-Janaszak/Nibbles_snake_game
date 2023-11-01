#include "engine.hpp"

void Engine::input() {
    Event event{};

    while (window.pollEvent(event)) {
        // Zamkni�cie okna
        if (event.type == Event::Closed) {
            window.close();
        }

        // Obs�uga klawiatury
        if (event.type == Event::KeyPressed) {
            // Wyj�cie
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }

            // Pauza
            if (Keyboard::isKeyPressed(Keyboard::P)) {
                togglePause();
            }

            // Nowa gra
            if (currentGameState == GameState::GAMEOVER) {
                if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                    startTheGame();
                }
            }
        }
    } // KONIEC p�tli while pollEvent

    // Kierunki
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        addDirection(Direction::UP);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Down)) {
        addDirection(Direction::DOWN);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left)) {
        addDirection(Direction::LEFT);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right)) {
        addDirection(Direction::RIGHT);
    }
}

void Engine::addDirection(int newDirection) {
    if (directionQueue.empty()) {
        directionQueue.emplace_back(newDirection);
    }
    else {
        if (directionQueue.back() != newDirection) {
            directionQueue.emplace_back(newDirection);
        }
    }
}
