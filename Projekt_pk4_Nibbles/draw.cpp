#include "engine.hpp"

void Engine::draw() {
    window.clear(Color::Black);

    // Rysowanie �cian
    for (auto& w : wallSections) {
        window.draw(w.getSprite());
    }

    // Rysowanie jab�ka
    window.draw(apple.getSprite());
    window.draw(superapple.getSprite());

    // Rysowanie sekcji w�a
    for (auto& s : snake) {
        window.draw(s.getSprite());
    }

    // Rysowanie tekstu
    window.draw(currentLevelText);
    window.draw(scoreText);

    // Rysowanie ekranu zako�czenia gry
    if (currentGameState == GameState::GAMEOVER) {
        window.draw(gameOverText);
        window.draw(pressEnterText);
    }

    window.display();
}
