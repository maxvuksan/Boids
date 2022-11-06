#pragma once

#include <SFML/Graphics.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
#include "CALC.h"

#include "Entity.h"
#include "QuadTree.h"

class Game {

	protected:
		
		int W_WIDTH;
		int W_HEIGHT;

		int D_WIDTH;
		int D_HEIGHT;

		sf::RenderWindow* WINDOW;
		sf::RenderTexture* DISPLAY;

		std::vector<sf::Color> FLOCK_COLOURS;
		int MAX_FLOCK_COUNT;

		sf::Clock CLOCK;
		sf::Color BACKGROUND_COLOUR;

		sf::Shader WOBBLE;

		sf::RectangleShape background_rect;
		int rect_count;

		float quad_tree_delay; //how frequently is the quad tree reconstructed
		float quad_tree_delay_tracked;

		std::vector<Entity*> ENTITIES;
		float SIMULATION_SPEED;

	public:
		int FLOCK_COUNT;
		int BOID_COUNT;

		bool SHOW_RADIUS;
		bool SHOW_TRAILS;

		float DELTA_TIME;
		float TIME;


		Game();
		~Game();

		sf::Color get_colour_by_flock(int);
		void reset_boids();

		void run();
		void start();
		void poll_events();
		void update();
		void render_ui();

		void add_entity(Entity*);
		std::vector<Entity*> get_ENTITIES();
		
		sf::Vector2i get_W_SIZE();
		sf::Vector2i get_D_SIZE();
};