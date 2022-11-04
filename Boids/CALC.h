#pragma once

#include <SFML/Graphics.hpp>

namespace CALC {

	int vector_to_angle(sf::Vector2f);

	int angle_between(sf::Vector2f, sf::Vector2f);
	sf::Vector2f vector_between(sf::Vector2f, sf::Vector2f);

	float distance(sf::Vector2f, sf::Vector2f);

	float magnitude(sf::Vector2f);
	sf::Vector2f normalize(sf::Vector2f);

	float clamp_upper(float, float);
	float clamp_lower(float, float);
	float clamp(float, float, float);
	sf::Vector2f clamp(sf::Vector2f, float, float);
}