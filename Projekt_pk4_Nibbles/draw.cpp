#include "engine.hpp"

void Engine::draw() {
    window.clear(Color::Black);

    // Rysowanie œcian
    for (auto& w : wallSections) {
        window.draw(w.getSprite());
    }

    // Rysowanie jab³ka
    window.draw(apple.getSprite());
    window.draw(superapple.getSprite());

    // Rysowanie sekcji wê¿a
    for (auto& s : snake) {
        window.draw(s.getSprite());
    }

    // Rysowanie tekstu
    window.draw(currentLevelText);
    window.draw(scoreText);

    // Rysowanie ekranu zakoñczenia gry
    if (currentGameState == GameState::GAMEOVER) {
        window.draw(gameOverText);
        window.draw(pressEnterText);
    }

    window.display();
}
