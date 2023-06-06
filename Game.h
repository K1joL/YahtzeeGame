#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include <string>

class Game
{
public:
	Game();
	~Game()
	{
	}
	void showDices(const Player &p) const;
	void keepDice(Player &p, const std::string &diceNumber);
	void rollDices(Player &p);
};

#endif //GAME_H