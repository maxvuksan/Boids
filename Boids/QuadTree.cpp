#include "QuadTree.h"
#include "CALC.h"
#include "Boid.h"
#include <iostream>

short QuadTree::capacity = 1;
short QuadTree::division_limit = 3;

QuadTree::QuadTree(sf::IntRect rect, short division_level){
	
	this->rect = rect;
	this->division_level = division_level;

	count = 0;
	sub_divided = false;
	drawn_rect.setOutlineColor(sf::Color(255, 255, 255, 50));
	drawn_rect.setOutlineThickness(1);
	drawn_rect.setFillColor(sf::Color::Transparent);
	drawn_rect.setSize(sf::Vector2f(rect.getSize().x, rect.getSize().y));
	drawn_rect.setPosition(sf::Vector2f(rect.left, rect.top));
}

void QuadTree::add_entity(Entity* entity) {

	if (position_in_tree(entity->position)) {

		if (count >= capacity && division_level < division_limit){
			if (!sub_divided) {
				sub_divide();
			}
			for (int i = 0; i < children.size(); i++) {
				children[i].add_entity(entity);
			}
			caught_entities.clear();
		}
		else {
			caught_entities.push_back(entity);
			count++;
		}
	}
}

bool QuadTree::position_in_tree(sf::Vector2f position) {
	if (position.x >= rect.left && position.x <= rect.left + rect.width) {
		if (position.y >= rect.top && position.y <= rect.top + rect.height) {
			return true;
		}
	}
	return false;
}

void QuadTree::draw(sf::RenderTexture& surface) {

	surface.draw(drawn_rect);

	for (int i = 0; i < children.size(); i++) {
		children[i].draw(surface);
	}
}

void QuadTree::sub_divide() {

	sub_divided = true;
	division_level += 1;

	children.emplace_back(sf::IntRect(rect.left, rect.top, rect.width / 2, rect.height / 2), division_level); //TOP LEFT
	children.emplace_back(sf::IntRect(rect.left + rect.width / 2, rect.top, rect.width / 2, rect.height / 2), division_level); //TOP RIGHT
	children.emplace_back(sf::IntRect(rect.left, rect.top + rect.height / 2, rect.width / 2, rect.height / 2), division_level); //BOTTOM LEFT
	children.emplace_back(sf::IntRect(rect.left + rect.width / 2, rect.top + rect.height / 2, rect.width / 2, rect.height / 2), division_level); //BOTTOM RIGHT
}

std::vector<Entity*> QuadTree::get_ENTITIES() {
	return caught_entities;
}

void QuadTree::purge_boids() {
	caught_entities.clear();
	for (int i = 0; i < children.size(); i++) {
		children[i].purge_boids();
	}
}

float QuadTree::left() {
	return rect.left;
}
float QuadTree::right() {
	return rect.left + rect.width;
}
float QuadTree::top() {
	return rect.top;
}
float QuadTree::bottom() {
	return rect.top + rect.height;
}

std::vector<Entity*> QuadTree::get_ENTITIES_in_range(sf::Vector2f position, float r, bool is_focus_boid) {

	std::vector<Entity*> boids_in_range;

	for (int i = 0; i < children.size(); i++) {
		auto children_boids = children[i].get_ENTITIES_in_range(position, r, is_focus_boid);
		boids_in_range.insert(boids_in_range.end(), children_boids.begin(), children_boids.end());
	}

	if (caught_entities.size() > 0) {
		//look for boids in this cell
		if (position.x + r > left() && position.x - r < right()) {
			if (position.y + r > top() && position.y - r < bottom()) {
				
				boids_in_range.insert(boids_in_range.end(), caught_entities.begin(), caught_entities.end());
			}
		}

	}

	return boids_in_range;
}