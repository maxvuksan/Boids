#include "Trail.h"
#include "Game.h"

float Trail::lifetime = 1;
int Trail::delay = 4;
float Trail::opacity = 0.3;

Trail::Trail(Entity* entity):
	shape(2)
{
	this->entity = entity;

	shape.setOrigin(sf::Vector2f(1, 1));
}

void Trail::set_colour(sf::Color colour) {
	this->colour = colour;
}

void Trail::update(sf::RenderTexture& surface) {

	if (delay_tracked > delay) {
		particles.push_back({entity->position, lifetime});
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i].lifetime -= entity->get_game()->DELTA_TIME;

		if (particles[i].lifetime <= 0) {
			particles.erase(particles.begin() + i);
		}

		shape.setFillColor(sf::Color(colour.r, colour.g, colour.b, particles[i].lifetime / lifetime * 255 * opacity));
		shape.setPosition(particles[i].position);

		surface.draw(shape);
	}

	delay_tracked++;
}

void Trail::purge_particles() {
	particles.clear();
}


