#pragma once

#include "Entity.h"
#include "Trail.h"


class Boid : public Entity {

	protected:

		int flock_index;
		int speed;

		sf::RectangleShape head_shape;
		static sf::CircleShape cohesion_shape;

		Trail trail;

		static int view_angle_lower_bound;
		static int view_angle_upper_bound;

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

		sf::Vector2f velocity;

		Boid();

		void start() override;
		void update(sf::RenderTexture&) override;

		int get_flock_index();

		void behaviour();
		void screen_wrap();
		void edge_bounce();
};