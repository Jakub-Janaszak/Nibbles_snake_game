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
 * Zwi�ksza numer poziomu, wczytuje kolejny poziom i resetuje w�a.
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

// To jest pocz�tkowy w�� tworzony na pocz�tku ka�dego poziomu
void Engine::newSnake() {
    snake.clear(); // Wyczyszczenie wektora 'snake' (usuni�cie poprzednich sekcji w�a)                
      
    // Utworzenie widoku (view) na sekwencj� liczb od 0 do 2                                           //<ranges>
    auto positions = std::views::iota(0, 3) | std::views::transform([](int i) {
        // Tworzenie pozycji dla kolejnych sekcji w�a
        return sf::Vector2f(100 - i * 20, 100);
        });

    // Wstawienie pozycji sekcji w�a do wektora 'snake'
    snake.insert(snake.end(), positions.begin(), positions.end());
}

void Engine::addSnakeSection() {
    // Po prostu dodaj sekcj� w tej samej pozycji co ostatnia sekcja w�a, b�dzie ona normalnie pod��a� za w�em         
    sf::Vector2f newSectionPosition = snake[snake.size() - 1].getPosition();
    snake.emplace_back(newSectionPosition);
}

void Engine::moveApple() {
    // Znajd� miejsce na umieszczenie jab�ka
    // Nie mo�e by� wewn�trz w�a ani na �cianie

    // Podziel pole na sekcje o rozmiarze jab�ka - odejmij 2, aby wykluczy� �ciany zewn�trzne
    sf::Vector2f appleResolution = sf::Vector2f(resolution.x / 20 - 2, resolution.y / 20 - 2);
    sf::Vector2f newAppleLocation;
    bool badLocation = false;
    srand(time(nullptr));
    // P�tla, dop�ki nie znajdziemy poprawnego miejsca
    do {
        badLocation = false;
        // Wygeneruj losowe miejsce
        newAppleLocation.x = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)appleResolution.x)) * 20;
        newAppleLocation.y = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)appleResolution.y)) * 20;

        // Sprawd�, czy jest w w�u
        for (auto& s : snake) {
            if (s.getSprite().getGlobalBounds().intersects(
                sf::FloatRect(newAppleLocation.x, newAppleLocation.y, 20, 20))) {
                badLocation = true;
                break;
            }
        }

        // Sprawd�, czy jest na �cianie
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
    // Znajd� miejsce na umieszczenie superjab�ka
    // Nie mo�e by� wewn�trz w�a ani na �cianie

    // Podziel pole na sekcje o rozmiarze superjab�ka - odejmij 2, aby wykluczy� �ciany zewn�trzne
    sf::Vector2f superAppleResolution = sf::Vector2f(resolution.x / 20 - 2, resolution.y / 20 - 2);
    sf::Vector2f newSuperAppleLocation;
    bool badLocation = false;
    srand(time(nullptr));

    // P�tla, dop�ki nie znajdziemy poprawnego miejsca dla superjab�ka
    do {
        badLocation = false;
        // Wygeneruj losowe miejsce dla superjab�ka
        newSuperAppleLocation.x = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)superAppleResolution.x)) * 20;
        newSuperAppleLocation.y = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)superAppleResolution.y)) * 20;

        // Sprawd�, czy jest w w�u
        for (auto& s : snake) {
            if (s.getSprite().getGlobalBounds().intersects(
                sf::FloatRect(newSuperAppleLocation.x, newSuperAppleLocation.y, 20, 20))) {
                badLocation = true;
                break;
            }
        }

        // Sprawd�, czy jest na �cianie
        for (auto& w : wallSections) {
            if (w.getSprite().getGlobalBounds().intersects(
                sf::FloatRect(newSuperAppleLocation.x, newSuperAppleLocation.y, 20, 20))) {
                badLocation = true;
                break;
            }
        }

        // Sprawd�, czy nowa lokalizacja koliduje z miejscem, gdzie jest jab�ko
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
 * Sprawdza plik manifestu poziom�w i upewnia si�, �e mo�emy otworzy� ka�dy plik poziomu.
 * Dodaje poprawne nazwy plik�w poziom�w do wektora 'levels' i zwi�ksza warto�� maxLevels.
 */
void Engine::checkLevelFiles() {
    // Wczytaj plik manifestu poziom�w
    if (std::filesystem::exists("assets/levels/levels.txt")) {                                                      //<filesystem>
        std::ifstream levelsManifest("assets/levels/levels.txt");

        std::ifstream testFile;
        for (std::string manifestLine; getline(levelsManifest, manifestLine);) {
            // Sprawd�, czy mo�na otworzy� plik poziomu
            testFile.open("assets/levels/" + manifestLine);
            if (testFile.is_open()) {
                // Plik poziomu zosta� otwarty, dodaj go do listy dost�pnych poziom�w
                levels.emplace_back("assets/levels/" + manifestLine);
                testFile.close();
                maxLevels++;
            }
        }
    }
}

/**
 * Wczytuje poziom z pliku i sprawdza znaki 'x' w celu dodania sekcji �ciany.
 * @param levelNumber int - numer wczytywanego poziomu. Plik powinien by� nazwany np. `level1.txt`
 */

void Engine::loadLevel(int levelNumber) {
    std::string levelFile = levels[levelNumber - 1];
    std::ifstream level(levelFile);
    std::string line;
    if (level.is_open()) {
        std::regex wallRegex("x"); // Wzorzec dla �ciany                                                             //<regex>
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

    // G��wna p�tla - dzia�a, dop�ki okno nie zostanie zamkni�te
    while (window.isOpen()) {
        sf::Time dt = clock.restart();

        if (currentGameState == GameState::PAUSED || currentGameState == GameState::GAMEOVER) {
            // Je�li jeste�my w trybie pauzy, sprawd� wprowadzanie (aby m�c wznowi� gr�), a nast�pnie przejd� do kolejnej iteracji p�tli
            input();

            // Narysuj ekran z informacj� o ko�cu gry
            if (currentGameState == GameState::GAMEOVER) {
                draw();
            }

            sf::sleep(sf::milliseconds(2)); // U�pij, aby nie obci��a� procesora
            continue;
        }

        timeSinceLastMove += dt;

        input();
        update();
        draw();
    }
}
