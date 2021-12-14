#pragma once
#include <optional>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "AnyArray.h"


class PerformanceMeasurer
{
public:
	virtual ~PerformanceMeasurer() = default;
	virtual std::pair<sf::Time, AnyArray> measure() = 0;

protected:
	sf::Clock clock;
};
