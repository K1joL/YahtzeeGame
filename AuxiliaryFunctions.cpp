#include "AuxiliaryFunctions.h"

std::vector<int> AuFunc::parseStringToInt(const std::string& string)
{
	std::vector<int> returnVector;
	returnVector.reserve(3);
	std::string tmp = "";
	for (auto& ch : string)
	{
		if (ch == ',')
		{
			returnVector.push_back(std::stoi(tmp));
			tmp = "";
			continue;
		}
		tmp += ch;
	}
	returnVector.push_back(std::stoi(tmp));

	return returnVector;
}

//solve the equation type : A*X+B*Y = overallSize
// returns a pair of two values: x and minY
std::vector<int> AuFunc::SizingMethod(int overallSize, int minX, int minY, float A, float B)
{
	int counter = 0;
	int offset = 0;
	float tmp = 0;
	do
	{
		tmp = ((overallSize - B * (minY + offset)) / A);
		offset++;
		counter++;
	} while (std::fmod(tmp * 10, 10) != 0 && counter < 25);

	return std::vector<int>{static_cast<int>(tmp), minY + --offset};
}

sf::Vector2f AuFunc::round(const sf::Vector2f &vector)
{
	return sf::Vector2f(std::round(vector.x), std::round(vector.y));
}