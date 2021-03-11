#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include "entity.h"
#include "button.h"

class GUI {
private:
	sf::Vector2f resolution;
	sf::Font font;
	std::vector<button> buttons;
public:
	void draw(sf::RenderWindow &window);
	void handleInput(sf::Event event, std::vector<entity> &entities);
	GUI(sf::Vector2f r);
	void addButton(std::string textString);
};