#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include "entity.h"

class button {
public:
	sf::Vector2f position, size;
	sf::Text text;
	sf::RectangleShape background;
	
	button(sf::Vector2f p_position, sf::Vector2f p_size, sf::Font &p_font, std::string textString);
	void draw(sf::RenderWindow &window);
	void activate(std::vector<entity> &entities);
};