#pragma once
#include "DynamicArray.h"
#include "Position.h"
#include "Person.h"

struct City
{
	DynamicArray<Person> people;

	void update(unsigned deltaTime)
	{
		for (unsigned i = 0; i < people.getSize(); ++i) {
			people[i].update(deltaTime);
		}
	}

	bool hasSickPeopleAround(Position const& pos, float radius)
	{
		for (unsigned i = 0; i < people.getSize(); ++i) {
			if (people[i].loc.checkIfHome()) continue;
			if (people[i].healthState.isHealthy()) continue;

			Position otherPos = people[i].loc.getPosition();

			if (withinRadius(pos, otherPos, radius)) return true;
		}

		return false;
	}
};
