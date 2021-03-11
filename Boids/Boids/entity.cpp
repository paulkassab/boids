#include "pch.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "entity.h"
#include <iostream>
#include <cmath>
using namespace sf;

int entity::total = 0;

entity::entity(int x, int y, sf::Font &font) {
	total += 1;
	id = total;
	left = false;
	right = false;
	forward = false;
	brake = false;
	inputControlled = false;
	accelerationPhysics = false;
	boolFriction = false;
	collision = false;
	rotationThrust = 100;
	thrust = 150;
	minFriction = 20;
	velocity.x = 0;
	velocity.y = 0;
	rotationSpeed = 0;
	position.x = x;
	position.y = y;
	facingAngle = 90;
	maxVelocity = 200;
	maxRotationSpeed = 200;
	info.setFont(font);
	info.setCharacterSize(20);
	info.setPosition(position);
	rayLength = 200;
	rayCount = 20;
	rays.setPrimitiveType(Lines);
	rays.resize(rayCount*2);

	//right
	for (int i = 0; i < rayCount; i+=2) {
		rays[i].position = position;
		rays[i].color = Color::Red;
		rays[i + 1].position.x = position.x + rayLength * cos( (facingAngle + i *4)* M_PI / 180);
		rays[i + 1].position.y = position.y + rayLength * sin( (facingAngle + i *4) * M_PI / 180);
		rays[i + 1].color = Color::Red;
	}
	//left
	for (int i = rayCount; i < 2 * rayCount -1 ; i += 2) {
		rays[i].position = position;
		rays[i].color = Color::Red;
		rays[i + 1].position.x = position.x + rayLength * cos((facingAngle - (i - rayCount) * 4)* M_PI / 180);
		rays[i + 1].position.y = position.y + rayLength * sin((facingAngle - (i - rayCount) * 4) * M_PI / 180);
		rays[i + 1].color = Color::Red;
	}

	shape.setPosition(position);
	shape.setPointCount(3);
	shape.setPoint(0, Vector2f(0,  0));
	shape.setPoint(1, Vector2f(- 20, -50));
	shape.setPoint(2, Vector2f(20, -50));
	shape.setFillColor(Color::Blue);
	
	
}


sf::VertexArray entity::getRays() {
	return rays;
}

sf::ConvexShape entity::getShape() {
	return shape;
}

void entity::draw(sf::RenderWindow &window) {
	window.draw(shape);
	window.draw(info);
	if (!inputControlled) {
		window.draw(rays);
	}
}

sf::Vector2f entity::getPosition() {
	return position;
}

void entity::switchControl() {
	inputControlled = !inputControlled;
	if (!inputControlled) {
		forward = false;
		left = false;
		right = false;
	}
}

void entity::handleInput(sf::Event event) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == Mouse::Left) {
			if (shape.getGlobalBounds().contains(Vector2f(event.mouseButton.x, event.mouseButton.y))) {
				switchControl();
			}
		}
	}

	if (inputControlled) {
		if (event.type == Event::KeyPressed) {
			if (event.key.code == Keyboard::A) {
				left = true;
			}
			if (event.key.code == Keyboard::D) {
				right = true;
			}
			if (event.key.code == Keyboard::W) {
				forward = true;
			}
			if (event.key.code == Keyboard::S) {
				brake = true;
			}
		}
		else if (event.type == Event::KeyReleased) {
			if (event.key.code == Keyboard::A) {
				left = false;
			}
			if (event.key.code == Keyboard::D) {
				right = false;
			}
			if (event.key.code == Keyboard::W) {
				forward = false;
			}
			if (event.key.code == Keyboard::S) {
				brake = false;
			}
		}
		
	}
}

void entity::rotateShape(float dts) {
	float rotationAngle = rotationSpeed *dts;
	facingAngle += rotationAngle;

	float theta = rotationAngle * M_PI / 180;
	Vector2f newPos;
	Vector2f pointPos = shape.getPoint(0);

	newPos.x = pointPos.x * cos(theta) - pointPos.y * sin(theta);
	newPos.y = pointPos.y * cos(theta) + pointPos.x * sin(theta);

	shape.setPoint(0, newPos);

	pointPos = shape.getPoint(1);

	newPos.x = pointPos.x * cos(theta) - pointPos.y * sin(theta);
	newPos.y = pointPos.y * cos(theta) + pointPos.x * sin(theta);

	shape.setPoint(1, newPos);

	pointPos = shape.getPoint(2);

	newPos.x = pointPos.x * cos(theta) - pointPos.y * sin(theta);
	newPos.y = pointPos.y * cos(theta) + pointPos.x * sin(theta);

	shape.setPoint(2, newPos);
}

void entity::applyVelocityFriction(float dts) {
	float velocityNorm = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
	float friction = thrust * (velocityNorm / maxVelocity) + minFriction;
	

	if (velocity.x > 0) {
		velocity.x -= (friction * dts) * (velocity.x / velocityNorm);
		if (velocity.x < 0) {
			velocity.x = 0;
		}
	}
	if (velocity.x < 0) {
		velocity.x += friction * dts * (std::abs(velocity.x) / velocityNorm);
		if (velocity.x > 0) {
			velocity.x = 0;
		}
	}
	if (velocity.y > 0) {
		velocity.y -= friction * dts * (velocity.y / velocityNorm);
		if (velocity.y < 0) {
			velocity.y = 0;
		}
	}
	if (velocity.y < 0) {
		velocity.y += friction * dts * (std::abs(velocity.y) / velocityNorm);
		if (velocity.y > 0) {
			velocity.y = 0;
		}
	}
}

void entity::applyRotationFriction(float dts) {
	float friction = rotationThrust * (std::abs(rotationSpeed) / maxRotationSpeed) + minFriction;
	
	if (rotationSpeed > 0) {
		rotationSpeed -= friction * dts;
	}
	if (rotationSpeed < 0) {
		rotationSpeed += friction * dts;
	}
}

void entity::applyFriction(float dts) {	
	applyVelocityFriction(dts);
	applyRotationFriction(dts);
}

void entity::moveLeft(float dts, float multiplier) {
	if (accelerationPhysics) {
		rotationSpeed -= rotationThrust * dts * multiplier;
	}
	else
	{
		rotationSpeed = -maxRotationSpeed * multiplier;
	}

	
}

void entity::moveRight(float dts, float multiplier) {
	if (accelerationPhysics) {
		rotationSpeed += rotationThrust * dts * multiplier;
	}
	else {
		rotationSpeed = maxRotationSpeed * multiplier;
	}
	
}

void entity::moveForward(float dts, float multiplier) {
	float theta = facingAngle * M_PI / 180;
	if (accelerationPhysics) {
		velocity.x += thrust * dts * cos(theta) * multiplier;
		velocity.y += thrust * dts * sin(theta) * multiplier;
	}
	else
	{
		velocity.x = maxVelocity * cos(theta) * multiplier;
		velocity.y = maxVelocity * sin(theta) * multiplier;
	}
}

void entity::boidAI(float dts, std::vector<entity> &entities) {
	
		left = false;
		right = false;
		forward = false;
		brake = false;
		float maxForwardMult = 0;
		float maxLeftMult = 0;
		float maxRightMult = 0;
		
	for (int i = 0; i < entities.size(); i++) { //loop through other entities
		
		if (entities[i].id == id) { continue; }
		
		float x1, y1, x2, y2, x3, y3, x4, y4;
		/*maxForwardMult = 0;
		maxLeftMult = 0;
		maxRightMult = 0;*/
		
		for (int k = 0; k < entities[i].shape.getPointCount(); k++) {
			
			x1 = entities[i].shape.getPoint(k).x + entities[i].getPosition().x;
			y1 = entities[i].shape.getPoint(k).y + entities[i].getPosition().y;
			if (k == entities[i].shape.getPointCount() - 1) {
				x2 = entities[i].shape.getPoint(0).x + entities[i].getPosition().x;
				y2 = entities[i].shape.getPoint(0).y + entities[i].getPosition().y;
			}
			else
			{
				x2 = entities[i].shape.getPoint(k + 1).x + entities[i].getPosition().x;
				y2 = entities[i].shape.getPoint(k + 1).y + entities[i].getPosition().y;
			}

			for (int j = rayCount - 1; j > 0; j -= 2) { //loop through right self rays
				x3 = rays[j - 1].position.x;
				y3 = rays[j - 1].position.y;
				x4 = rays[j].position.x;
				y4 = rays[j].position.y;


				//perform check:
				float num = (x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4);
				float t = num / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
				num = (x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3);
				float u = num / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));

				if ((t >= 0 && t <= 1) && (u >= 0 && u <= 1)) {
					//intersection exists
					//can calculate intersection coordinates
					float xx = x1 + t * (x2 - x1) - position.x;
					float yy = y1 + t * (y2 - y1) - position.y;
					
					if (maxRightMult < j / (rayCount - 1)) {
						maxRightMult = j / (rayCount - 1);
					}
					if (maxForwardMult < sqrt(xx * xx + yy * yy) / rayLength) {
						maxForwardMult = sqrt(xx * xx + yy * yy) / rayLength;
					}
				}
			}

			for (int j = 2 * rayCount - 1; j > rayCount - 1; j -= 2) { //loop through left self rays
				x3 = rays[j - 1].position.x;
				y3 = rays[j - 1].position.y;
				x4 = rays[j].position.x;
				y4 = rays[j].position.y;


				//perform check:
				float num = (x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4);
				float t = num / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
				num = (x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3);
				float u = num / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));

				if ((t >= 0 && t <= 1) && (u >= 0 && u <= 1)) {
					//intersection exists
					//can calculate intersection coordinates
					float xx = x1 + t * (x2 - x1) - position.x;
					float yy = y1 + t * (y2 - y1) - position.y;
					if (maxLeftMult < (j - rayCount) / (rayCount - 1)) {
						maxLeftMult = (j - rayCount) / (rayCount - 1);
					}
					if (maxForwardMult < sqrt(xx * xx + yy * yy) / rayLength) {
						maxForwardMult = sqrt(xx * xx + yy * yy) / rayLength;
					}
				}
			}		
		}

		
	}

	if (maxLeftMult > maxRightMult) {
		moveLeft(dts, maxLeftMult - maxRightMult);
	}
	else if (maxRightMult > maxLeftMult) {
		moveRight(dts, maxRightMult - maxLeftMult);
	}
	else
	{
		if (!accelerationPhysics) {
			rotationSpeed = 0;
		}
	}
	moveForward(dts, maxForwardMult);
} 

float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return abs((x1*(y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

float pointDistance(float x1, float y1, float x2, float y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	return sqrt(dx*dx + dy * dy);
}

bool pointOnLine(float x1, float y1, float x2, float y2, float x, float y) {
	return pointDistance(x1, y1, x, y) + pointDistance(x2, y2, x, y) == pointDistance(x1, y1, x2, y2);
}

void entity::collisionDetection(std::vector<entity> &entities) {

	for (int k = 0; k < shape.getPointCount(); k++) {

		float x = shape.getPoint(k).x + position.x;
		float y = shape.getPoint(k).y + position.y;
		//wall collision
			
		if (x < 0) {
			position.x -= x;
			velocity.x = -velocity.x;
		}
		if (x > 1366) {
			position.x -= x - 1366;
			velocity.x = -velocity.x;
		}
		if (y < 0) {
			position.y -= y;
			velocity.y = -velocity.y;
		}
		if (y > 768) {
			position.y -= y - 768;
			velocity.y = -velocity.y;
		}

		float x1, y1, x2, y2, x3, y3, x4, y4;
		for (int i = 0; i < entities.size(); i++) { //loop through other entities
			if (entities[i].id == id) { continue; }
				
			x1 = entities[i].shape.getPoint(0).x + entities[i].position.x;
			y1 = entities[i].shape.getPoint(0).y + entities[i].position.y;

			x2 = entities[i].shape.getPoint(1).x + entities[i].position.x;
			y2 = entities[i].shape.getPoint(1).y + entities[i].position.y;

			x3 = entities[i].shape.getPoint(2).x + entities[i].position.x;
			y3 = entities[i].shape.getPoint(2).y + entities[i].position.y;

			//check if this->vertex is in entities[i].triangle
			if (triangleArea(x1, y1, x2, y2, x3, y3) == triangleArea(x, y, x2, y2, x3, y3) + triangleArea(x1, y1, x, y, x3, y3) + triangleArea(x1, y1, x2, y2, x, y)) {
				//if (!pointOnLine(x1, y1, x2, y2, x, y) && !pointOnLine(x1, y1, x3, y3, x, y) && !pointOnLine(x3, y3, x2, y2, x, y)) {
				//intersection confirmed
				//find line intersection between this->vertex's edges and entities[i]'s edges
				if (k == 2) {
					x1 = shape.getPoint(1).x + position.x;
					y1 = shape.getPoint(1).y + position.y;
					x2 = shape.getPoint(0).x + position.x;
					y2 = shape.getPoint(0).y + position.y;
				}
				else if (k == 0) {
					x1 = shape.getPoint(2).x + position.x;
					y1 = shape.getPoint(2).y + position.y;
					x2 = shape.getPoint(1).x + position.x;
					y2 = shape.getPoint(1).y + position.y;
				}
				else
				{
					x1 = shape.getPoint(0).x + position.x;
					y1 = shape.getPoint(0).y + position.y;
					x2 = shape.getPoint(2).x + position.x;
					y2 = shape.getPoint(2).y + position.y;
				}

				float dx = 0, dy = 0;

				for (int j = 0; j < 3; j++) {
					x3 = entities[i].shape.getPoint(j).x + entities[i].position.x;
					y3 = entities[i].shape.getPoint(j).y + entities[i].position.y;
					if (j == entities[i].shape.getPointCount() - 1) {
						x4 = entities[i].shape.getPoint(0).x + entities[i].getPosition().x;
						y4 = entities[i].shape.getPoint(0).y + entities[i].getPosition().y;
					}
					else
					{
						x4 = entities[i].shape.getPoint(j + 1).x + entities[i].getPosition().x;
						y4 = entities[i].shape.getPoint(j + 1).y + entities[i].getPosition().y;
					}

					float num = (x - x3)*(y3 - y4) - (y - y3)*(x3 - x4);
					float t = num / ((x - x2)*(y3 - y4) - (y - y2)*(x3 - x4));
					num = (x2 - x)*(y - y3) - (y2 - y)*(x - x3);
					float u = num / ((x - x2)*(y3 - y4) - (y - y2)*(x3 - x4));

					if ((t >= 0 && t <= 1) && (u >= 0 && u <= 1)) {
						//intersection exists
						//can calculate intersection coordinates
						float xx = x + t * (x2 - x);
						float yy = y + t * (y2 - y);
						dx += (xx - x) / 2;
						dy += (yy - y) / 2;
					}

					num = (x - x3)*(y3 - y4) - (y - y3)*(x3 - x4);
					t = num / ((x - x1)*(y3 - y4) - (y - y1)*(x3 - x4));
					num = (x1 - x)*(y - y3) - (y1 - y)*(x - x3);
					u = num / ((x - x1)*(y3 - y4) - (y - y1)*(x3 - x4));

					if ((t >= 0 && t <= 1) && (u >= 0 && u <= 1)) {
						//intersection exists
						//can calculate intersection coordinates
						float xx = x + t * (x1 - x);
						float yy = y + t * (y1 - y);
						dx += (xx - x) / 2;
						dy += (yy - y) / 2;
					}
				}

				position.x += dx/2;
				position.y += dy/2;
				entities[i].position.x -= dx / 2;
				entities[i].position.y -= dy / 2;
				float tempx = velocity.x;
				float tempy = velocity.y;
				velocity.x += (entities[i].velocity.x - velocity.x) / 2 ;
				velocity.y += (entities[i].velocity.y - velocity.y) / 2 ;
				entities[i].velocity.x += (tempx - entities[i].velocity.x) / 2 ;
				entities[i].velocity.y += (tempy - entities[i].velocity.y) / 2 ;
				//}

			}

		}

	}

}


void entity::update(float dts, std::vector<entity> &entities) {
	
	if (!inputControlled) {
		boidAI(dts, entities);
	}
	else {
		if (right) {
			moveRight(dts, 1);
		}
		else if (left)
		{
			moveLeft(dts, 1);
		}
		else if (!accelerationPhysics)
		{
			rotationSpeed = 0;
		}

		if (forward) {
			moveForward(dts, 1);
		}
		else if (!accelerationPhysics)
		{
			velocity.x = 0;
			velocity.y = 0;
		}
		if (brake && accelerationPhysics) {
			applyFriction(dts);
		}
	}
	



	if (boolFriction) {
		applyFriction(dts);
	}
	
	


	rotateShape(dts);
	float dx = velocity.x * dts;
	float dy = velocity.y * dts;
	position.x += dx;  //////////////////////////////////// final entity position update
	position.y += dy;  ////////////////////////////////////
	
	
	//collision detection
	if (collision) {
		collisionDetection(entities);
	}


	//right
	for (int i = 0; i < rayCount; i += 2) {
		rays[i].position = position;
		
		rays[i + 1].position.x = position.x + rayLength * cos((facingAngle + i * 4)* M_PI / 180); 
		rays[i + 1].position.y = position.y + rayLength * sin((facingAngle + i * 4) * M_PI / 180);
		
	}
	//left
	for (int i = rayCount; i < 2 * rayCount - 1; i += 2) {
		rays[i].position = position;
		
		rays[i + 1].position.x = position.x + rayLength * cos((facingAngle - (i - rayCount) * 4)* M_PI / 180);
		rays[i + 1].position.y = position.y + rayLength * sin((facingAngle - (i - rayCount) * 4) * M_PI / 180);
		
	}
	info.setPosition(position);
	info.setString(std::to_string(id));
	shape.setPosition(position);
}