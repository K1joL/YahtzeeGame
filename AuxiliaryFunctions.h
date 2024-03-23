#ifndef MATHSFUNCTIONS_H
#define MATHSFUNCTIONS_H

#include <vector>
#include <string>
#include <SFML\Graphics.hpp>

namespace AuFunc 
{
	std::vector<int> parseStringToInt(const std::string& string);
	std::vector<int> SizingMethod(int overallSize, int minX, int minY, float A, float B);
	sf::Vector2f round(const sf::Vector2f &vector);
} //AuFunc

#endif // !MATHSFUNCTIONS_H
