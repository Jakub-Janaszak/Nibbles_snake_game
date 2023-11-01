#include "graphicObject.hpp"

GraphicObject::GraphicObject(Vector2f position, Vector2f size) {
	sprite.setSize(size);
	sprite.setPosition(position);
}

void GraphicObject::setPosition(Vector2f newPosition) {
	sprite.setPosition(newPosition);
}

RectangleShape GraphicObject::getSprite() {
	return sprite;
}

void GraphicObject::setColor(sf::Color color) {

	sprite.setFillColor(color);
}