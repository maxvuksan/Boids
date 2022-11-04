#include "Boid.h"
#include "Game.h"

int Boid::edge_buffer = 50;
float Boid::edge_strength = 0.05f;

float Boid::seperation_strength = 40;
float Boid::seperation_radius = 25;

float Boid::alignment_limit = 10;

float Boid::cohesion_strength = 4.5;
float Boid::cohesion_radius = 100;

float Boid::target_speed = 50;
int Boid::speed_variance = 5;

Boid::Boid() :
	trail(this) {}

void Boid::start() {

	speed = rand() % (speed_variance) + target_speed - speed_variance;

	flock_index = rand() % game->FLOCK_COUNT;

	trail.set_colour(game->get_colour_by_flock(flock_index));

	head_shape.setSize(sf::Vector2f(6, 2));
	head_shape.setOrigin(sf::Vector2f(3, 1));
	head_shape.setFillColor(game->get_colour_by_flock(flock_index));

	cohesion_shape.setRadius(cohesion_radius);
	cohesion_shape.setOrigin(sf::Vector2f(cohesion_radius, cohesion_radius));
	cohesion_shape.setOutlineColor(sf::Color(255, 255, 255, 50));
	cohesion_shape.setOutlineThickness(1);
	cohesion_shape.setFillColor(sf::Color::Transparent);

	position = sf::Vector2f(rand() % 700, rand() % 700);
	velocity = sf::Vector2f(static_cast <float> (rand()) / static_cast <float> (2) - 1, static_cast <float> (rand()) / static_cast <float> (2) - 1);
	velocity = CALC::normalize(velocity);
	tag = "boid";
}

int Boid::get_flock_index() {
	return flock_index;
}

void Boid::update(sf::RenderTexture& surface) {

	if (game->SHOW_TRAILS) {
		trail.update(surface);
	}
	else {
		trail.purge_particles();
	}

	behaviour();

	edge_bounce();

	rotation = CALC::vector_to_angle(velocity);
	head_shape.setRotation(rotation);

	cohesion_shape.setPosition(position);
	head_shape.setPosition(position);

	//grounding any oversteps in velocity, keeping a consistent speed
	velocity = CALC::normalize(velocity);
	position += velocity * (float)speed * game->DELTA_TIME;

	if (game->SHOW_RADIUS) {
		surface.draw(cohesion_shape);
	}
	surface.draw(head_shape);
}

void Boid::behaviour() {

	std::vector<Boid*> local_boids;
	std::vector<Boid*> seperation_boids;

	auto quad_entities = game->QUAD_TREE.get_ENTITIES_in_range(position, cohesion_radius);

	for (int i = 0; i < quad_entities.size(); i++) {

		float distance = CALC::distance(quad_entities[i]->position, position);
		if (quad_entities[i]->tag == "boid" && distance < cohesion_radius && quad_entities[i] != this) {

			auto boid = dynamic_cast<Boid*>(quad_entities[i]);
			local_boids.push_back(boid);

			if (distance < seperation_radius) {
				seperation_boids.push_back(boid);
			}
		}
	}

	sf::Vector2f average_position(0, 0);
	int local_count = 0;

	sf::Vector2f alignment(0, 0);
		
	for (int i = 0; i < local_boids.size(); i++) {
			//only join if they are of the same flock
		if (flock_index == local_boids[i]->get_flock_index()) {
			average_position += local_boids[i]->position;
			alignment += local_boids[i]->velocity;
			local_count++;
		}
	}

	if (local_count > 0) {
		//ALIGNMENT
		alignment /= (float)local_count;
		alignment = alignment - velocity;
		alignment = CALC::clamp(alignment, -alignment_limit, alignment_limit);
		velocity += alignment * game->DELTA_TIME;

		//COHESION
		average_position /= (float)local_count;
		velocity += CALC::vector_between(position, average_position) * game->DELTA_TIME * cohesion_strength;
	}

	//SEPERATION
	if (seperation_boids.size() > 0) {
		
		sf::Vector2f seperation(0, 0);
		float total_distance = 0;
		for (int i = 0; i < seperation_boids.size(); i++) {
			float distance = CALC::distance(seperation_boids[i]->position, position);
			total_distance += distance;
			seperation += CALC::normalize(CALC::vector_between(seperation_boids[i]->position, position)) * (seperation_radius - distance);
		}
		if (total_distance != 0) {
			seperation /= total_distance;
		}
		velocity += seperation * game->DELTA_TIME * seperation_strength;
	}
}

void Boid::screen_wrap() {
	if (position.x > game->get_D_SIZE().x + edge_buffer) {
		position.x = -edge_buffer;
	}
	else if (position.x < -edge_buffer) {
		position.x = game->get_D_SIZE().x + edge_buffer;
	}

	if (position.y > game->get_D_SIZE().y + edge_buffer) {
		position.y = -edge_buffer;
	}
	else if (position.y < -edge_buffer) {
		position.y = game->get_D_SIZE().y + edge_buffer;
	}
}

void Boid::edge_bounce() {
	if (position.x > game->get_D_SIZE().x - edge_buffer) {
		velocity.x -= edge_strength;
	}
	if (position.x < edge_buffer) {
		velocity.x += edge_strength;
	}

	if (position.y > game->get_D_SIZE().y - edge_buffer) {
		velocity.y -= edge_strength;
	}
	if (position.y < edge_buffer) {
		velocity.y += edge_strength;
	}
}
