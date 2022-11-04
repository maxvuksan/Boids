#pragma once
#include <SFML/Graphics.hpp>

class Entity;


struct Particle {
	sf::Vector2f position;
	float lifetime;
};

class Trail {

	protected:
		Entity* entity;
		std::vector<Particle> particles;

		sf::CircleShape shape;

		sf::Color colour;
		static float opacity;
		static float lifetime;
		static int delay;

		int delay_tracked;

	public:
		Trail(Entity*);

		void set_colour(sf::Color);
		void update(sf::RenderTexture&);

		void purge_particles();
};