#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <map>
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
		void unkeep() { m_isKeeped = false; }
		void roll();
		bool operator==(const Dice& right) const { return this->getTop() == right.getTop(); }
		bool operator<(const Dice& right) const { return this->getTop() < right.getTop(); }

	};

	struct Score
	{
		int score = 0;
		bool isFilled = false;
	};
private:
	std::vector<Dice> m_dices;
	std::map<std::string, Score> m_scoreTable;
	int m_score = 0;
	static int m_id;

private:
	void _createTable();

public:
	Player()
	{
		for (int i = 0; i < 5; i++)
			m_dices.push_back( Dice{ i + 1 } );
		_createTable();
		m_id++;
	}
	~Player()
	{
	}
	int getID() const { return m_id; }
	std::vector<Dice>& getDices() { return m_dices; }
	const std::vector<Dice>& getDices() const { return m_dices; }
	std::map<std::string, Score> getScoresTable(){ return m_scoreTable; }
	const std::map<std::string, Score> getScoresTable() const { return m_scoreTable; }
	int getScores() const { return m_score; }
	void earnScores(int scores, std::string combo);
	bool isUpperSectionFilled() const;
	bool isOneKeeped();
};

#endif //PLAYER_H