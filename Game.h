#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>

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

private:
	std::map<std::string, Combo> m_comboMap;
	void initComboMap();
public:
	Game()
	{
		initComboMap();
	}
	~Game()
	{
	}
	void startGame();
	void showScores(const std::vector<Player> &pQueue) const;
	void whoWins(const std::vector<Player> &pQueue) const;
	std::vector<int> showDices(const Player &p) const;
	void keepDice(Player &p, const std::string &diceNumber);
	void unkeepDice(Player &p, const std::string& diceNumber);
	void unkeepAll(Player &p);
	void rollDices(Player &p);
	void showCombos();
	bool isValidCombo(const Player& p, std::string combo);
	void takeCombo(Player &p, std::string &combo, bool joker = false);
	bool isYahtzee(Player &p);
};

#endif //GAME_H