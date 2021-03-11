#include "pch.h"
#include "button.h"

using namespace sf;

button::button(sf::Vector2f p_position, sf::Vector2f p_size, sf::Font &p_font, std::string textString) {
	
	position = p_position;
	size = p_size;
	text.setCharacterSize(size.y / 2);
	text.setFont(p_font);
	text.setString(textString);
	text.setPosition(position);
	background.setPosition(position);
	background.setFillColor(sf::Color::Red);
	background.setSize(size);
}
void button::draw(sf::RenderWindow &window) {
	window.draw(background);
	window.draw(text); //////////
}

void button::activate(std::vector<entity> &entities) {
	if (text.getString() == "Physics") {
		if (background.getFillColor() == Color::Red) {
			background.setFillColor(Color::Green);
			for (int i = 0; i < entities.size(); i++) {
				entities[i].accelerationPhysics = true;
			}
		}
		else
		{
			background.setFillColor(Color::Red);
			for (int i = 0; i < entities.size(); i++) {
				entities[i].accelerationPhysics = false;
			}
		}
	}
	else if (text.getString() == "Friction") {
		if (background.getFillColor() == Color::Red) {
			background.setFillColor(Color::Green);
			for (int i = 0; i < entities.size(); i++) {
				entities[i].boolFriction = true;
			}
		}
		else
		{
			background.setFillColor(Color::Red);
			for (int i = 0; i < entities.size(); i++) {
				entities[i].boolFriction = false;
			}
		}
	}
	else if (text.getString() == "Collision") {
		if (background.getFillColor() == Color::Red) {
			background.setFillColor(Color::Green);
			for (int i = 0; i < entities.size(); i++) {
				entities[i].collision = true;
			}
		}
		else
		{
			background.setFillColor(Color::Red);
			for (int i = 0; i < entities.size(); i++) {
				entities[i].collision = false;
			}
		}
	}
}