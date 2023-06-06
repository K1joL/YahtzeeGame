#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <random>
#include <time.h>

class Player
{
	class Dice
	{
		int m_top = 1;
		bool m_isKeeped = 0;
	public:
		Dice(int top) : m_top{top}
		{
			//Random number generator for roll()
			srand((unsigned)time(NULL));
		}
		~Dice() {}
		int getTop() const { return m_top; }
		bool isKeeped() const { return m_isKeeped; }
		void keep() { m_isKeeped = true; }
		void roll();
	};

private:
	std::vector<Dice> m_dices;

public:
	Player()
	{
		for (int i = 0; i < 5; i++)
			m_dices.push_back( Dice{ i + 1 } );
	}
	~Player()
	{
	}
	std::vector<Dice>& getDices() { return m_dices; }
	const std::vector<Dice>& getDices() const { return m_dices; }
};

#endif //PLAYER_H