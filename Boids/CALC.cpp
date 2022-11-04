#include "CALC.h"


int CALC::vector_to_angle(sf::Vector2f vector) {
	return round(atan2(vector.y, vector.x) * (180/ 3.14));
}

int CALC::angle_between(sf::Vector2f a, sf::Vector2f b) {
	return vector_to_angle(b - a);
}

sf::Vector2f CALC::vector_between(sf::Vector2f a, sf::Vector2f b) {
	return normalize(b - a);
}

float CALC::distance(sf::Vector2f a, sf::Vector2f b) {
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

float CALC::magnitude(sf::Vector2f v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

sf::Vector2f CALC::normalize(sf::Vector2f v) {
	float scaler = magnitude(v);
	if (scaler != 0) {
		v /= magnitude(v);
	}
	return v;
}

float CALC::clamp_upper(float v, float max) {
	if (v > max) {
		v = max;
	}
	return v;
}
float CALC::clamp_lower(float v, float min) {
	if (v < min) {
		v = min;
	}
	return v;
}
float CALC::clamp(float v, float min, float max) {
	return clamp_lower(clamp_upper(v, max), min);
}

sf::Vector2f CALC::clamp(sf::Vector2f v, float min, float max) {
	return sf::Vector2f(clamp(v.x, min, max), clamp(v.y, min, max));
}