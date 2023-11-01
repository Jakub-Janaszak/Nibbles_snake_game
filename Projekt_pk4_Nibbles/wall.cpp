#include "wall.hpp"


Wall::Wall(sf::Vector2f position, sf::Vector2f size) : GraphicObject(position, size) {
    setColor(Color::White);
}

//Wall::Wall(Vector2f position, Vector2f size) {
//	wallShape.setSize(size);
//	wallShape.setFillColor(Color::White);
//	wallShape.setPosition(position);
//}
//
//RectangleShape Wall::getShape() {
//	return wallShape;
//}