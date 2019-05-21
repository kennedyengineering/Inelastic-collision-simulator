#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include <array>
#include <iostream>
#include <cmath>

class Object : public sf::CircleShape {
private:
	double _mass;
	std::array<double, 2> _velocity;
	std::array<double, 2> _newVelocity;
	bool collisionDetected = false;
public:
	Object(double radius, double mass, std::array<double, 2> position, std::array<double, 2> velocity) : sf::CircleShape(radius), _mass(mass), _velocity(velocity) {
		sf::CircleShape::setPosition(position[0], position[1]);
		sf::CircleShape::setFillColor(sf::Color(200,20,20));
	}

	double getMomentumX() {
		return _mass * _velocity[0];
	}
	double getMomentumY() {
		return _mass * _velocity[1];
	}
	double getKineticEnergyX() {
		//Ke = 0.5m*v^2
		return 0.5 * _mass * std::pow(_velocity[0], 2);
	}
	double getKineticEnergyY() {
		return 0.5 * _mass * std::pow(_velocity[1], 2);
	}
	double getMass() {
		return _mass;
	}
	double getVelocityX() {
		return _velocity[0];
	}
	double getVelocityY() {
		return _velocity[1];
	}
	void setVelocityX(double newVelocityX) {
		_newVelocity[0] = newVelocityX;
	}
	void setVelocityY(double newVelocityY) {
		_newVelocity[1] = newVelocityY;
	}
	void setStatus(bool status) {
		collisionDetected = status;
	}

	void setNewVelocity() {
		if (collisionDetected == true) {
			_velocity = _newVelocity;
			collisionDetected = false;
		}
	}

	void detectCollision(sf::RenderWindow &window, std::vector<Object> &objectVector) {
		//detect collision
		//change velocity
		
		//detect walls
		const double winSizeX = window.getSize().x;
		const double winSizeY = window.getSize().y;
		const double objPosX = sf::CircleShape::getPosition().x;
		const double objPosY = sf::CircleShape::getPosition().y;
		const double radius = sf::CircleShape::getRadius();
		const double centerX = objPosX + radius;
		const double centerY = objPosY + radius;

		if (winSizeX <= (objPosX+(2*radius))) 
			_velocity[0] *= -1; //perfectly elastic collision
		if (objPosX <= 0)
			_velocity[0] *= -1;

		if (winSizeY <= (objPosY+(2*radius)))
			_velocity[1] *= -1;
		if (objPosY <= 0)
			_velocity[1] *= -1;

		//detect other Objects
		for (int i = 0; i < objectVector.size(); i++) {
			if (&objectVector[i] == this) //check for self
				continue;
			else {
				//detect collision
				double otherPosX = objectVector[i].getPosition().x;
				double otherPosY = objectVector[i].getPosition().y;
				double otherRadius = objectVector[i].getRadius();
				double otherCenterX = otherPosX + otherRadius;
				double otherCenterY = otherPosY + otherRadius;

				double distanceX = std::abs(otherCenterX - centerX);
				double distanceY = std::abs(otherCenterY - centerY);
				double distance = std::sqrt((distanceX*distanceX) + (distanceY*distanceY));

				if ((distance <= (radius+otherRadius)) && collisionDetected == false) {
					//collision detected
					collisionDetected = true;
					objectVector[i].setStatus(true);
					//work out new velocity
					//_velocity[0] *= -1;
					//_velocity[1] *= -1;
					double momentumX = getMomentumX(); 	//_mass * _velocity[0];
					double momentumY = getMomentumY(); 	//_mass * _velocity[1];
					double energyX = getKineticEnergyX();
					double energyY = getKineticEnergyY();
					double otherMomentumX = objectVector[i].getMomentumX();
					double otherMomentumY = objectVector[i].getMomentumY();
					double otherEnergyX = objectVector[i].getKineticEnergyX();
					double otherEnergyY = objectVector[i].getKineticEnergyY();
					double otherMass = objectVector[i].getMass();
					double otherVelocityX = objectVector[i].getVelocityX();
					double otherVelocityY = objectVector[i].getVelocityY();

					//elastic collision
					//energy and momentum are conserved
					// Pin = Pout
					// KEin = KEout
					// Vf1 = (m1-m2)Vi1/(m1+m2) //this object?
					// Vf2 = 2*m1*Vi1/(m1+m2)   //or this one?
					// X and Y independent?	

					//need to implement a collision list. change the velocities of both objects at the same time. Vf1 and VF2
					
					//Vf1
					//FAIL
					double otherfinalVelocityX = (_mass - otherMass)*_velocity[0]/(_mass+otherMass);
					double otherfinalVelocityY = (_mass - otherMass)*_velocity[1]/(_mass+otherMass);
				
					//FAILS ON SECOND COLLISION
					//double otherfinalVelocityX = (otherMass - _mass)*otherVelocityX/(otherMass+_mass);
					//double otherfinalVelocityY = (otherMass - _mass)*otherVelocityY/(otherMass+_mass);
					objectVector[i].setVelocityX(otherfinalVelocityX);
					objectVector[i].setVelocityY(otherfinalVelocityY);

					//Vf2
					//RESULTS! - KEEPS GETTING SLOWER...
					double finalVelocityX = 2*otherMass*otherVelocityX/(otherMass*_mass);
					double finalVelocityY = 2*otherMass*otherVelocityY/(otherMass*_mass);

					//FAIL
					//double finalVelocityX = 2*_mass*_velocity[0]/(_mass*otherMass);
					//double finalVelocityY = 2*_mass*_velocity[1]/(_mass*otherMass);

					//set new velocity later, will mess with other objects calculations
					//example:
					//_newVelocity[0] = _velocity[0]*-1;
					//_newVelocity[1] = _velocity[1]*-1;
					_newVelocity[0] = finalVelocityX;
					_newVelocity[1] = finalVelocityY;

					//std::cout << finalVelocityX << " " << finalVelocityY << std::endl;
				}		
			}
		}		
	}

	void updatePosition() {
		sf::CircleShape::move(_velocity[0], _velocity[1]);	
	}
};

class Engine {
private:
	sf::RenderWindow *windowReference; //doesnt like being const!
	std::vector<Object> objectVector;
public:
	Engine(sf::RenderWindow &window) {
		windowReference = &window;
	}

	void createObject(double radius, double mass,  std::array<double, 2> position, std::array<double, 2> velocity) {
		Object object(radius, mass, position, velocity);
		objectVector.push_back(object);
	}

	void render() {
		windowReference->clear(sf::Color::Black);
		
		for (int i = 0; i < objectVector.size(); i++) //handle collision calculations
			objectVector[i].detectCollision(*windowReference, objectVector);

		for (int i = 0; i < objectVector.size(); i++) { //move objects
			//objectVector[i].detectCollision(*windowReference, objectVector);
			objectVector[i].setNewVelocity();
			objectVector[i].updatePosition();
			windowReference->draw(objectVector[i]);
		}

		windowReference->display();
	}

	~Engine() {
	}
};

int main() {
	//window setup
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML TEST");
	window.setVerticalSyncEnabled(true); //set to monitors frequency

	Engine engine(window);

	// Lines are weird
	/*	
	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(10.f, 10.f)),
		sf::Vertex(sf::Vector2f(150.f, 150.f))
	};
	*/

	//radius, mass, position, velocity
	engine.createObject(10.0, 10.0, std::array<double, 2> {80,20}, std::array<double, 2> {0.0,-1.0});
	engine.createObject(20.0, 1.0, std::array<double, 2> {70, 90}, std::array<double, 2> {0.0,0.0});

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	
		engine.render();
		
	}

	return 0;
}
