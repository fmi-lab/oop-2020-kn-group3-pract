#include "Person.h"
#include "City.h"

void Person::update(float deltaTime)
{
	healthState.update(deltaTime);

	if (loc.checkIfHome()) {
		if (!healthState.hasSympoms && myrand() > 0.9)
			loc.goOut({20, 20, rand() * 2 * PI});
	} else { // if outside
		loc.move(4);

		if (rand() > 0.5) loc.goHome();

		if (city.hasSickPeopleAround(loc.getPosition(), 10) && rand() > 0.3) {
			healthState.infect();
		}
	}
}
