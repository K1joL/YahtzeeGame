#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include <string>
#include <iostream>
#include <iomanip>

class Game
{
	enum Combo
	{
		Ones,
		Twos,
		Threes,
		Fours,
		Fives,
		Sixes,
		ThreeOfAKind,
		FourOfAKind,
		FullHouse,
		SmallStraight,
		LargeStraight,
		Chance,
		Yahtzee,
		MAX_COMBOS
	};

public:
	Game()
	{
	}
	~Game()
	{
	}
	void showDices(const Player &p) const;
	void keepDice(Player &p, const std::string &diceNumber);
	void rollDices(Player &p);
	void showCombos();
};

#endif //GAME_H