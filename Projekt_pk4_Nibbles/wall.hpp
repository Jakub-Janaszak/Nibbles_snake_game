#ifndef NIBBLES_03_WALL_HPP
#define NIBBLES_03_WALL_HPP

#include <SFML/Graphics.hpp>
#include "graphicObject.hpp"

using namespace sf;

class Wall : public GraphicObject{
//private:
//    
//    RectangleShape wallShape;
//
public:
	Wall(sf::Vector2f position, sf::Vector2f size);
//    Wall(Vector2f position, Vector2f size);
//
//    RectangleShape getShape();

};


#endif //NIBBLES_03_WALL_HPP