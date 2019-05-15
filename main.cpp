#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include <array>
#include <iostream>

class Object : public sf::CircleShape {
private:
	double _mass;
	std::array<double, 2> _velocity;
public:
	Object(double radius, double mass, std::array<double, 2> position, std::array<double, 2> velocity) : sf::CircleShape(radius), _mass(mass), _velocity(velocity) {
		sf::CircleShape::setPosition(position[0], position[1]);
		sf::CircleShape::setFillColor(sf::Color(200,20,20));
	}

	void updatePosition() { 
		//std::cout << sf::CircleShape::getPosition().x << " " << sf::CircleShape::getPosition().y << std::endl;
		sf::CircleShape::move(_velocity[0], _velocity[1]);
		//std::cout << (sf::CircleShape::getPosition()).x << " " << (sf::CircleShape::getPosition()).y << std::endl;
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
		
		for (int i = 0; i < objectVector.size(); i++) {
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

	// Circles
	//sf::CircleShape shape(50.f);
	//shape.setFillColor(sf::Color(100, 250, 50));

	// Lines are weird
	/*	
	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(10.f, 10.f)),
		sf::Vertex(sf::Vector2f(150.f, 150.f))
	};
	*/

	//radius, mass, position, velocity
	engine.createObject(10.0, 10.0, std::array<double, 2> {20,20}, std::array<double, 2> {0.5,0});

	/*
	std::vector<Object> vec;

	Object obj(10,10,std::array<double, 2> {20,20}, std::array<double, 2> {0.01,0});
	vec.push_back(obj);
	*/

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	
		//window.clear(sf::Color::Black);
		//shape.move(1,0);
		//window.draw(shape);
		//window.draw(line, 2, sf::Lines);

		//WORKS!!!
		//vec[0].updatePosition();
		//window.draw(vec[0]);
		//other std::for_each and enhanced for loop dont work --> return a copy?
		//just use vector::size and loop through them all!

		engine.render();
		//window.display();
	}

	return 0;
}
