#include "engine.hpp"


const sf::Time Engine::TimePerFrame = sf::seconds(1.f / 60.f);

Engine::Engine() {
    resolution = sf::Vector2f(800, 600);
    window.create(sf::VideoMode(resolution.x, resolution.y), "Nibbles", sf::Style::Default);
    window.setFramerateLimit(FPS);
    maxLevels = 0;
    checkLevelFiles();
    startTheGame();

    mainFont.loadFromFile("assets/fonts/PressStart2P-Regular.ttf");

    setupText(&currentLevelText, mainFont, "level 1", 14, sf::Color::Black);
    currentLevelText.setPosition(sf::Vector2f(15, 3));
    sf::FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();

    setupText(&scoreText, mainFont, std::to_string(score), 14, sf::Color::Black);
    sf::FloatRect scoreTextBounds = scoreText.getLocalBounds();
    scoreText.setPosition(sf::Vector2f(resolution.x - scoreTextBounds.width - 15, 3));

    setupText(&gameOverText, mainFont, "GAME OVER", 36, sf::Color::Yellow);
    sf::FloatRect gameOverTextBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(sf::Vector2f(resolution.x / 2 - gameOverTextBounds.width / 2, 100));
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(2);

    setupText(&pressEnterText, mainFont, "Nacisnij ENTER, aby zagrac ponownie", 20, sf::Color::Green);
    sf::FloatRect pressEnterTextBounds = pressEnterText.getLocalBounds();
    pressEnterText.setPosition(sf::Vector2f(resolution.x / 2 - pressEnterTextBounds.width / 2, 200));
    pressEnterText.setOutlineColor(sf::Color::Black);
    pressEnterText.setOutlineThickness(2);

    currentGameState = GameState::RUNNING;
    lastGameState = currentGameState;
}

void Engine::startTheGame() {
    score = 0;
    speed = 2;
    snakeDirection = Direction::RIGHT;
    sectionsToAdd = 0;
    timeSinceLastMove = sf::Time::Zero;
    wallSections.clear();
    directionQueue.clear();
    applesEatenThisLevel = 0;
    applesEatenTotal = 0;
    currentLevel = 1;
    loadLevel(currentLevel);
    newSnake();
    moveApple();
    moveSuperApple();
    currentGameState = GameState::RUNNING;
    lastGameState = currentGameState;
    currentLevelText.setString("Poziom " + std::to_string(currentLevel));
    sf::FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
    scoreText.setString(std::to_string(score));
    sf::FloatRect scoreTextBounds = scoreText.getLocalBounds();
    scoreText.setPosition(sf::Vector2f(resolution.x - scoreTextBounds.width - 15, 3));
}

/**
 * Zwiêksza numer poziomu, wczytuje kolejny poziom i resetuje wê¿a.
 */
void Engine::beginNextLevel() {
    currentLevel += 1;
    wallSections.clear();
    directionQueue.clear();
    speed = 2 + currentLevel;
    snakeDirection = Direction::RIGHT;
    sectionsToAdd = 0;
    applesEatenThisLevel = 0;

    loadLevel(currentLevel);
    newSnake();
    moveApple();
    moveSuperApple();
    currentLevelText.setString("Poziom: " + std::to_string(currentLevel));
    sf::FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
}

// To jest pocz¹tkowy w¹¿ tworzony na pocz¹tku ka¿dego poziomu
void Engine::newSnake() {
    snake.clear(); // Wyczyszczenie wektora 'snake' (usuniêcie poprzednich sekcji wê¿a)                
      
    // Utworzenie widoku (view) na sekwencjê liczb od 0 do 2                                           //<ranges>
    auto positions = std::views::iota(0, 3) | std::views::transform([](int i) {
        // Tworzenie pozycji dla kolejnych sekcji wê¿a
        return sf::Vector2f(100 - i * 20, 100);
        });

    // Wstawienie pozycji sekcji wê¿a do wektora 'snake'
    snake.insert(snake.end(), positions.begin(), positions.end());
}

void Engine::addSnakeSection() {
    // Po prostu dodaj sekcjê w tej samej pozycji co ostatnia sekcja wê¿a, bêdzie ona normalnie pod¹¿aæ za wê¿em         
    sf::Vector2f newSectionPosition = snake[snake.size() - 1].getPosition();
    snake.emplace_back(newSectionPosition);
}

void Engine::moveApple() {
    // ZnajdŸ miejsce na umieszczenie jab³ka
    // Nie mo¿e byæ wewn¹trz wê¿a ani na œcianie

    // Podziel pole na sekcje o rozmiarze jab³ka - odejmij 2, aby wykluczyæ œciany zewnêtrzne
    sf::Vector2f appleResolution = sf::Vector2f(resolution.x / 20 - 2, resolution.y / 20 - 2);
    sf::Vector2f newAppleLocation;
    bool badLocation = false;
    srand(time(nullptr));
    // Pêtla, dopóki nie znajdziemy poprawnego miejsca
    do {
        badLocation = false;
        // Wygeneruj losowe miejsce
        newAppleLocation.x = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)appleResolution.x)) * 20;
        newAppleLocation.y = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)appleResolution.y)) * 20;

        // SprawdŸ, czy jest w wê¿u
        for (auto& s : snake) {
            if (s.getSprite().getGlobalBounds().intersects(
                sf::FloatRect(newAppleLocation.x, newAppleLocation.y, 20, 20))) {
                badLocation = true;
                break;
            }
        }

        // SprawdŸ, czy jest na œcianie
        for (auto& w : wallSections) {
            if (w.getSprite().getGlobalBounds().intersects(
                sf::FloatRect(newAppleLocation.x, newAppleLocation.y, 20, 20))) {
                badLocation = true;
                break;
            }
        }

    } while (badLocation);
    apple.setPosition(newAppleLocation);
    
}

void Engine::moveSuperApple() {
    // ZnajdŸ miejsce na umieszczenie superjab³ka
    // Nie mo¿e byæ wewn¹trz wê¿a ani na œcianie

    // Podziel pole na sekcje o rozmiarze superjab³ka - odejmij 2, aby wykluczyæ œciany zewnêtrzne
    sf::Vector2f superAppleResolution = sf::Vector2f(resolution.x / 20 - 2, resolution.y / 20 - 2);
    sf::Vector2f newSuperAppleLocation;
    bool badLocation = false;
    srand(time(nullptr));

    // Pêtla, dopóki nie znajdziemy poprawnego miejsca dla superjab³ka
    do {
        badLocation = false;
        // Wygeneruj losowe miejsce dla superjab³ka
        newSuperAppleLocation.x = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)superAppleResolution.x)) * 20;
        newSuperAppleLocation.y = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)superAppleResolution.y)) * 20;

        // SprawdŸ, czy jest w wê¿u
        for (auto& s : snake) {
            if (s.getSprite().getGlobalBounds().intersects(
                sf::FloatRect(newSuperAppleLocation.x, newSuperAppleLocation.y, 20, 20))) {
                badLocation = true;
                break;
            }
        }

        // SprawdŸ, czy jest na œcianie
        for (auto& w : wallSections) {
            if (w.getSprite().getGlobalBounds().intersects(
                sf::FloatRect(newSuperAppleLocation.x, newSuperAppleLocation.y, 20, 20))) {
                badLocation = true;
                break;
            }
        }

        // SprawdŸ, czy nowa lokalizacja koliduje z miejscem, gdzie jest jab³ko
        if (apple.getSprite().getGlobalBounds().contains(newSuperAppleLocation)) {
            badLocation = true;
        }

    } while (badLocation);

    superapple.setPosition(newSuperAppleLocation);
}

void Engine::togglePause() {
    if (currentGameState == GameState::RUNNING) {
        lastGameState = currentGameState;
        currentGameState = GameState::PAUSED;
    }
    else if (currentGameState == GameState::PAUSED) {
        currentGameState = lastGameState;
    }
}

void Engine::setupText(sf::Text* textItem, const sf::Font& font, const sf::String& value, int size, sf::Color colour) {
    textItem->setFont(font);
    textItem->setString(value);
    textItem->setCharacterSize(size);
    textItem->setFillColor(colour);
}

/**
 * Sprawdza plik manifestu poziomów i upewnia siê, ¿e mo¿emy otworzyæ ka¿dy plik poziomu.
 * Dodaje poprawne nazwy plików poziomów do wektora 'levels' i zwiêksza wartoœæ maxLevels.
 */
void Engine::checkLevelFiles() {
    // Wczytaj plik manifestu poziomów
    if (std::filesystem::exists("assets/levels/levels.txt")) {                                                      //<filesystem>
        std::ifstream levelsManifest("assets/levels/levels.txt");

        std::ifstream testFile;
        for (std::string manifestLine; getline(levelsManifest, manifestLine);) {
            // SprawdŸ, czy mo¿na otworzyæ plik poziomu
            testFile.open("assets/levels/" + manifestLine);
            if (testFile.is_open()) {
                // Plik poziomu zosta³ otwarty, dodaj go do listy dostêpnych poziomów
                levels.emplace_back("assets/levels/" + manifestLine);
                testFile.close();
                maxLevels++;
            }
        }
    }
}

/**
 * Wczytuje poziom z pliku i sprawdza znaki 'x' w celu dodania sekcji œciany.
 * @param levelNumber int - numer wczytywanego poziomu. Plik powinien byæ nazwany np. `level1.txt`
 */

void Engine::loadLevel(int levelNumber) {
    std::string levelFile = levels[levelNumber - 1];
    std::ifstream level(levelFile);
    std::string line;
    if (level.is_open()) {
        std::regex wallRegex("x"); // Wzorzec dla œciany                                                             //<regex>
        for (int y = 0; y < 30; y++) {
            getline(level, line);
            std::sregex_iterator regexIt(line.begin(), line.end(), wallRegex);
            std::sregex_iterator regexEnd;
            for (; regexIt != regexEnd; ++regexIt) {
                int x = regexIt->position();
                wallSections.emplace_back(Wall(sf::Vector2f(x * 20, y * 20), sf::Vector2f(20, 20)));
            }
        }
    }
    level.close();

}

void Engine::run() {
    sf::Clock clock;

    // G³ówna pêtla - dzia³a, dopóki okno nie zostanie zamkniête
    while (window.isOpen()) {
        sf::Time dt = clock.restart();

        if (currentGameState == GameState::PAUSED || currentGameState == GameState::GAMEOVER) {
            // Jeœli jesteœmy w trybie pauzy, sprawdŸ wprowadzanie (aby móc wznowiæ grê), a nastêpnie przejdŸ do kolejnej iteracji pêtli
            input();

            // Narysuj ekran z informacj¹ o koñcu gry
            if (currentGameState == GameState::GAMEOVER) {
                draw();
            }

            sf::sleep(sf::milliseconds(2)); // Uœpij, aby nie obci¹¿aæ procesora
            continue;
        }

        timeSinceLastMove += dt;

        input();
        update();
        draw();
    }
}
