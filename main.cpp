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

				if (distance <= (radius+otherRadius)) {
					//collision detected
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
					
					//elastic collision
					//energy and momentum are conserved
					// Pin = Pout
					// KEin = KEout
					
					
					//set new velocity later, will mess with other objects calculations
					//example:
					collisionDetected = true;
					_newVelocity[0] = _velocity[0]*-1;
					_newVelocity[1] = _velocity[1]*-1;
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
	engine.createObject(10.0, 10.0, std::array<double, 2> {80,20}, std::array<double, 2> {1.0,-1.0});
	engine.createObject(20.0, 5.0, std::array<double, 2> {70, 80}, std::array<double, 2> {0.5,1.0});

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
