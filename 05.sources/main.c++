#include "DynamicArray.h"
#include <iostream>
#include <locale>
#include "Position.h"
#include "City.h"
#include "Person.h"

using namespace std;

struct City;

int main()
{
	PersonLocation loc;
	City city;
	city.hasSickPeopleAround(loc.getPosition(), 10);
	loc.move(10);

	Person x{city, loc, HealthState::ILL};

	x.update(10);

	return 0;
}
