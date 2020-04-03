#pragma once

#include "Position.h"
#include <stdexcept>

struct HealthState
{
	float timeIll;
	bool hasSympoms;

	enum { HEALTHY, ILL, RECOVERED } healthState;

	void update(float deltaTime)
	{
		if (healthState == ILL) timeIll += deltaTime;
	}

	bool isHealthy() { return healthState == HEALTHY; }

	void infect()
	{
		healthState = ILL;
		timeIll = 0;
		hasSympoms = false;
	}
};

class PersonLocation
{
	union {
		Position pos;   // 3 * 4 byte
		float timeHome; // 4 byte ......
	};
	bool isHome;

public:
	void goHome()
	{
		isHome = true;
		timeHome = 0;
	}

	void move(float distance)
	{
		if (!isHome)
			pos.move(distance);
		else
			throw std::runtime_error("Can't move if isHome");
	}

	Position getPosition()
	{
		if (!isHome)
			return pos;
		else
			throw std::runtime_error(
			    "Can't get position of person who is home");
	}

	bool checkIfHome() { return isHome; }

	void goOut(Position const& loc)
	{
		isHome = false;
		pos = loc;
	}
};

// 0 <= myrand() <= 1
inline float myrand() { return 0.5; }

struct City;

struct Person
{
	City& city;

	PersonLocation loc;
	HealthState healthState;

	void update(float deltaTime);
};

