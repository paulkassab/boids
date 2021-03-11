#include "pch.h"
#include "GUI.h"
#include "SFML/Graphics.hpp"
#include <iostream>
using namespace sf;

GUI::GUI(Vector2f r) {
	resolution = r;
	font.loadFromFile("arial.ttf");
}

void GUI::addButton(std::string textString) {
	
	sf::Vector2f size(40, 20);
	button a(sf::Vector2f(resolution.x - size.x - 5, (5 + size.y) * buttons.size()) , size, font, textString);
	buttons.push_back(a);
}

void GUI::draw(sf::RenderWindow &window) {
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].draw(window);
	}
}

void GUI::handleInput(Event event, std::vector<entity> &entities) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == Mouse::Left) {
			for (int i = 0; i < buttons.size(); i++) {
				if (buttons[i].background.getGlobalBounds().contains(Vector2f(event.mouseButton.x, event.mouseButton.y))) {
					buttons[i].activate(entities);
				}
			}
		}
	}
}