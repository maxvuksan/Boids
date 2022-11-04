#pragma once

#include <SFML/Graphics.hpp>


class Game;

class Entity {

	protected:
		Game* game;

	public:
		std::string tag;
		sf::Vector2f position;
		short rotation;


		Entity();
		
		Game* get_game();
		void link_game(Game*);

		virtual void start(){}
		virtual void update(){}
		virtual void update(sf::RenderTexture&){}
		virtual void catch_event(sf::Event){}

		virtual ~Entity(){}
};