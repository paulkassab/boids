#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include <set>

class entity {
private:
	static int total;
	int id;
	std::set<int> nearby;
	sf::ConvexShape shape;
	sf::VertexArray rays;
	sf::Vector2f position, velocity;
	sf::Text info;
	float rotationThrust, rotationSpeed, thrust, facingAngle, minFriction, maxVelocity, maxRotationSpeed, rayLength, rayCount;
	bool left, right, forward, brake, inputControlled;
	void moveLeft(float dts, float multiplier);
	
	void moveRight(float dts, float multiplier);
	
	void moveForward(float dts, float multiplier);
	
	void switchControl();
	void boidAI(float dts, std::vector<entity> &entities);
	void rotateShape(float dts);
	void applyFriction(float dts);
	void applyVelocityFriction(float dts);
	void collisionDetection(std::vector<entity> &entities);
	void applyRotationFriction(float dts);
	sf::ConvexShape getShape();
	sf::Vector2f getPosition();
	sf::VertexArray getRays();

public:
	entity(int x, int y, sf::Font &font);		
	void handleInput(sf::Event event);
	void update(float dts, std::vector<entity> &entities);
	void draw(sf::RenderWindow &window);
	bool accelerationPhysics, boolFriction, collision;

	

	

};