#include "snakesection.hpp"

SnakeSection::SnakeSection(Vector2f position) : GraphicObject(position) {
	setColor(Color::Green);
	positionSS = position;
}


void SnakeSection::update() {
	setPosition(positionSS);
}


void SnakeSection::setPositionSS(Vector2f newPosition){
	positionSS = newPosition;
}


//SnakeSection::SnakeSection(Vector2f startPosition) {
//	section.setSize(Vector2f(20, 20));
//	section.setFillColor(Color::Green);
//	section.setPosition(startPosition);
//	position = startPosition;
//}
//
Vector2f SnakeSection::getPosition() {
	return positionSS;
}
//
//
//RectangleShape SnakeSection::getShape() {
//	return section;
//}
//
//void SnakeSection::update() {
//	section.setPosition(position);
//}