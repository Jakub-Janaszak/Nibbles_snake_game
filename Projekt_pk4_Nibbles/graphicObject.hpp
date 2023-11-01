#ifndef NIBBLES_04_GRAPHICOBJECT_HPP
#define NIBBLES_04_GRAPHICOBJECT_HPP

#include <SFML/Graphics.hpp>

using namespace sf;

class GraphicObject {
private:
    RectangleShape sprite;

public:
    GraphicObject(Vector2f position = Vector2f(400, 300), Vector2f size = Vector2f(20, 20));

    RectangleShape getSprite();
    void setPosition(Vector2f newPosition);
    void setColor(sf::Color  color); 
};


#endif //NIBBLES_04_WALL_HPP