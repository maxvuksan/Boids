#pragma once

#include "Entity.h"
#include "Trail.h"


class Boid : public Entity {

	protected:

		int flock_index;
		int speed;

		sf::Vector2f velocity;

		sf::RectangleShape head_shape;
		sf::CircleShape cohesion_shape;

		Trail trail;

		static int edge_buffer;
		static float edge_strength;

		static float seperation_radius;
		static float seperation_strength;

		static float alignment_limit;

		static float cohesion_strength;
		static float cohesion_radius;

		static float target_speed;
		static int speed_variance;

	public:
		Boid();

		void start() override;
		void update(sf::RenderTexture&) override;

		int get_flock_index();

		void behaviour();
		void screen_wrap();
		void edge_bounce();
};