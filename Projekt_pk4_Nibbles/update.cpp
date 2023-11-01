#include "engine.hpp"

void Engine::update() {
    // Aktualizuj pozycje wê¿a
    if (timeSinceLastMove.asSeconds() >= seconds(1.f / float(speed)).asSeconds()) {
        Vector2f thisSectionPosition = snake[0].getPosition();
        Vector2f lastSectionPosition = thisSectionPosition;

        // Czy coœ znajduje siê w kolejce wejœciowej?
        if (!directionQueue.empty()) {
            // Upewnij siê, ¿e nie zmieniamy kierunku na przeciwny
            switch (snakeDirection) {
            case Direction::UP:
                if (directionQueue.front() != Direction::DOWN) {
                    snakeDirection = directionQueue.front();
                }
                break;
            case Direction::DOWN:
                if (directionQueue.front() != Direction::UP) {
                    snakeDirection = directionQueue.front();
                }
                break;
            case Direction::LEFT:
                if (directionQueue.front() != Direction::RIGHT) {
                    snakeDirection = directionQueue.front();
                }
                break;
            case Direction::RIGHT:
                if (directionQueue.front() != Direction::LEFT) {
                    snakeDirection = directionQueue.front();
                }
                break;
            }
            directionQueue.pop_front();
        }

        // Aktualizuj wynik
        //score += snake.size() * (applesEatenTotal + 1);
        

        // Czy powinniœmy powiêkszyæ wê¿a?
        if (sectionsToAdd) {
            addSnakeSection();
            sectionsToAdd--;
        }

        // Aktualizuj pozycjê g³owy wê¿a
        switch (snakeDirection) {
        case Direction::RIGHT:
            snake[0].setPositionSS(Vector2f(thisSectionPosition.x + 20, thisSectionPosition.y));
            break;
        case Direction::DOWN:
            snake[0].setPositionSS(Vector2f(thisSectionPosition.x, thisSectionPosition.y + 20));
            break;
        case Direction::LEFT:
            snake[0].setPositionSS(Vector2f(thisSectionPosition.x - 20, thisSectionPosition.y));
            break;
        case Direction::UP:
            snake[0].setPositionSS(Vector2f(thisSectionPosition.x, thisSectionPosition.y - 20));
            break;
        }

        // Aktualizuj pozycjê ogona wê¿a
        for (int s = 1; s < snake.size(); s++) {
            thisSectionPosition = snake[s].getPosition();
            snake[s].setPositionSS(lastSectionPosition);
            lastSectionPosition = thisSectionPosition;
        }

        // Aktualizuj sekcje wê¿a
        for (auto& s : snake) {
            s.update();
        }

        // Wykrywanie kolizji - Jab³ko
        if (snake[0].getSprite().getGlobalBounds().intersects(apple.getSprite().getGlobalBounds())) {
            // Uderzyliœmy w jab³ko, dodaj wiêcej sekcji do wê¿a, zwiêksz prêdkoœæ i przesuñ jab³ko
            applesEatenThisLevel += 1;
            applesEatenTotal += 1;
            /////////////////aktualizacja wyniku/////////////////
            score += applesEatenThisLevel;
            scoreText.setString(to_string(score));
            FloatRect scoreTextBounds = scoreText.getLocalBounds();
            scoreText.setPosition(Vector2f(resolution.x - scoreTextBounds.width - 15, 3));
            ////////////////////////////////////////////////////
            FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();

            bool beginningNewLevel = false;
            if (applesEatenThisLevel >= 10) {
                // Rozpocznij nastêpny poziom, jeœli istniej¹ kolejne,
                // w przeciwnym razie pozostañ na ostatnim poziomie i utrudniaj grê, a¿ umrzemy.
                if (currentLevel < maxLevels) {
                    beginningNewLevel = true;
                    beginNextLevel();
                }
            }
            if (!beginningNewLevel) {
                sectionsToAdd += applesEatenThisLevel;
                speed++;
                moveApple();
            }
        }

        // Wykrywanie kolizji - Super Jab³ko
        if (snake[0].getSprite().getGlobalBounds().intersects(superapple.getSprite().getGlobalBounds())) {
            // Uderzyliœmy w jab³ko, dodaj wiêcej sekcji do wê¿a, zwiêksz prêdkoœæ i przesuñ jab³ko
            applesEatenThisLevel += 1;
            applesEatenTotal += 1;
            /////////////////aktualizacja wyniku/////////////////
            score += (applesEatenThisLevel*2);
            scoreText.setString(to_string(score));
            FloatRect scoreTextBounds = scoreText.getLocalBounds();
            scoreText.setPosition(Vector2f(resolution.x - scoreTextBounds.width - 15, 3));
            ////////////////////////////////////////////////////
            FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();

            bool beginningNewLevel = false;
            if (applesEatenThisLevel >= 10) {
                // Rozpocznij nastêpny poziom, jeœli istniej¹ kolejne,
                // w przeciwnym razie pozostañ na ostatnim poziomie i utrudniaj grê, a¿ umrzemy.
                if (currentLevel < maxLevels) {
                    beginningNewLevel = true;
                    beginNextLevel();
                }
            }
            if (!beginningNewLevel) {
                sectionsToAdd += applesEatenThisLevel;
                speed+=2;
                moveSuperApple();
            }
        }


        // Wykrywanie kolizji - Cia³o wê¿a
        for (int s = 1; s < snake.size(); s++) {
            if (snake[0].getSprite().getGlobalBounds().intersects(snake[s].getSprite().getGlobalBounds())) {
                // Koniec gry
                currentGameState = GameState::GAMEOVER;
            }
        }

        // Wykrywanie kolizji - Œciana
        for (auto& w : wallSections) {
            if (snake[0].getSprite().getGlobalBounds().intersects(w.getSprite().getGlobalBounds())) {
                // Koniec gry
                currentGameState = GameState::GAMEOVER;
            }
        }

        // Zresetuj czas od ostatniego ruchu.
        timeSinceLastMove = Time::Zero;
    } // KONIEC aktualizacji pozycji wê¿a
}
