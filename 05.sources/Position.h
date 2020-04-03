#pragma once

#include <cmath>

const float PI = 3.1415;

struct Position
{
	float x, y;
	float angle; // radians

	void move(float distance)
	{
		x += distance * cos(angle);
		y += distance * sin(angle);
	}
};

inline float distanceSquared(Position a, Position b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline bool withinRadius(Position a, Position b, float r)
{
	return distanceSquared(a, b) <= r * r;
}
