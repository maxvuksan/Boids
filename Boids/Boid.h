#pragma once

#include "Entity.h"
#include "Trail.h"


class Boid : public Entity {

	protected:

		int flock_index;
		int speed;

		sf::RectangleShape head_shape;
		sf::CircleShape cohesion_shape;

		sf::RectangleShape quad_tree_overlap_graphic;

		Trail trail;

		bool focus_boid;

		static int view_angle_lower_bound;
		static int view_angle_upper_bound;

		static int edge_buffer;
		static float edge_strength;

		int detection_dimensions;
		int nearby_boids;

		static float seperation_radius;
		static float seperation_strength;

		static float alignment_limit;

		static float cohesion_strength;
		static float cohesion_radius;

		static float target_speed;
		static int speed_variance;

	public:

		sf::Vector2f velocity;

		static int required_nearby_boids;

		Boid();

		void start() override;
		void update(sf::RenderTexture&) override;
		void update_overlay(sf::RenderTexture&) override;

		int get_flock_index();

		void behaviour();
		void screen_wrap();
		void edge_bounce();
};