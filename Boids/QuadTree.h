#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class QuadTree {
	protected:

		short count;
		static short capacity;
		static short division_limit;

		short division_level;
		bool sub_divided;

		std::vector<QuadTree> children;
		std::vector<Entity*> caught_entities;

		sf::RectangleShape drawn_rect;

		sf::IntRect rect;

	public:

		QuadTree(sf::IntRect, short = 0);

		void purge_boids(); //removes the dead refereces from caught entities

		void add_entity(Entity*); //adds entity into the appropriate region
		void sub_divide();

		bool position_in_tree(sf::Vector2f position);

		void draw(sf::RenderTexture&);

		std::vector<Entity*> get_ENTITIES();
		std::vector<Entity*> get_ENTITIES_in_range(sf::Vector2f position, float range);

		float left();
		float right();
		float top();
		float bottom();

};