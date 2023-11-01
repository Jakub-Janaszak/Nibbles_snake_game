
#ifndef NIBBLES_01_SNAKESECTION_HPP
#define NIBBLES_01_SNAKESECTION_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
#include "graphicObject.hpp"

class SnakeSection : public GraphicObject {
private:
    Vector2f positionSS;
    RectangleShape section;
public:
    explicit SnakeSection(Vector2f position);
    Vector2f getPosition();
    void setPositionSS(Vector2f newPosition);
    void update();
};


#endif //NIBBLES_01_SNAKESECTION_HPP


//class SnakeSection {
//private:
//    Vector2f position;
//    RectangleShape section;
//public:
//    explicit SnakeSection(Vector2f startPosition);
//
//    Vector2f getPosition();
//    void setPosition(Vector2f newPosition);
//
//    RectangleShape getShape();
//
//    void update();
//};