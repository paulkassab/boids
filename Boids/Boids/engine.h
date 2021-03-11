#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "entity.h"
#include "GUI.h"

class engine {
private:
	sf::RenderWindow window;
	std::vector<entity> entities;
	sf::Font font;
	sf::Text fps, pausetxt;
	sf::RectangleShape pauseShape;
	GUI* gui;
	entity* playerEntity;
	sf::Vector2f resolution;
	void input();
	void update(float dt);
	void draw();
	bool pause;
	

public:
	engine();
	void start();
};