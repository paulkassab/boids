#include "pch.h"
#include "engine.h"
#include <iostream> 

using namespace sf;

engine::engine() {
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	window.create(VideoMode(resolution.x, resolution.y), "Boids", Style::Fullscreen);
	window.setKeyRepeatEnabled(false);
	gui = new GUI(resolution);
	gui->addButton("Physics");
	gui->addButton("Friction");
	gui->addButton("Collision");
	font.loadFromFile("arial.ttf");
	fps.setFont(font);
	fps.setCharacterSize(20);
	fps.setPosition(10, 10);

	pauseShape.setFillColor(Color::Red);
	pauseShape.setSize(Vector2f(50, 20));
	pauseShape.setPosition(10, 40);
	pausetxt.setFont(font);
	pausetxt.setCharacterSize(15);
	pausetxt.setPosition(10, 40);
	pausetxt.setString("Pause");
	playerEntity = NULL;

	////////////////////////////////////////
	
}

void engine::start() {
	
	
	Clock clock;
	while (window.isOpen())
	{
		Time dt = clock.restart();
		float dts = dt.asSeconds();
		input();		
		update(dts);
		draw();
	}

	delete gui;
	gui = NULL;
}

void engine::input() {
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed) {
			window.close();
		}

		if (event.type == Event::KeyPressed) {
			if (event.key.code == Keyboard::Escape) {
				window.close();
			}
		}
		
		gui->handleInput(event, entities);

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == Mouse::Right) {
				entity a(event.mouseButton.x, event.mouseButton.y, font);
				entities.push_back(a);				
			}	
			if (event.mouseButton.button == Mouse::Left) {
				if (pauseShape.getGlobalBounds().contains(Vector2f(event.mouseButton.x, event.mouseButton.y))) {
					if (pauseShape.getFillColor() == Color::Red) {
						pause = true;
						pauseShape.setFillColor(Color::Green);
					}
					else
					{
						pause = false;
						pauseShape.setFillColor(Color::Red);
					}
				}
			}
		}

		for (int i = 0; i < entities.size(); i++) {
			entities[i].handleInput(event);
		}		
	}
}





void engine::update(float dts) {
	if (!pause) {
		for (int i = 0; i < entities.size(); i++) {
			entities[i].update(dts, entities);
		}
	}

	float FPS = 1 / dts;
	fps.setString(std::to_string(FPS));
}

void engine::draw() {
	window.clear();
	
	
	for (int i = 0; i < entities.size(); i++) {
		entities[i].draw(window);
	}
	
	window.draw(fps);
	window.draw(pauseShape);
	window.draw(pausetxt);
	gui->draw(window);
	window.display();
	}