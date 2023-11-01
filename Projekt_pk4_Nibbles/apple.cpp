#include "apple.hpp"


Apple::Apple() : GraphicObject() {
    setColor(Color::Red);
}

//Apple::Apple() {
//	Vector2f startingPosition(400, 300);
//	sprite.setSize(Vector2f(20, 20));
//	sprite.setFillColor(Color::Red);
//	sprite.setPosition(startingPosition);
//}
//
//void Apple::setPosition(Vector2f newPosition) {
//	sprite.setPosition(newPosition);
//}
//
//RectangleShape Apple::getSprite() {
//	return sprite;
//}
//
//void Apple::setColor(sf::Color color) {
//	
//	sprite.setFillColor(color);
//}