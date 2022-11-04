#include "Entity.h"


Entity::Entity(): game(nullptr) {}

void Entity::link_game(Game* game) {
	this->game = game;
}

Game* Entity::get_game() {
	return game;
}