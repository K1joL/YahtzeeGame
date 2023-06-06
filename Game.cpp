#include "Game.h"
#include <iostream>

using namespace std;

void Game::showDices(const Player &p) const
{
	int index = 1;
	for (auto &dice : p.getDices())
	{
		cout << index << " Dice - " << dice.getTop();
		if (dice.isKeeped())
			cout << "\tkeeped";
		cout << endl;
		index++;
	}
	cout << endl;
}

void Game::keepDice(Player &p, const std::string &diceNumber)
{
	for (int i = 0; i < diceNumber.length(); i++)
	{
		int diceIndex = diceNumber[i] - '0';
		p.getDices()[diceIndex].keep();
	}
}

void Game::rollDices(Player &p)
{
	for (auto& dice : p.getDices())
		if (dice.isKeeped() == false)
			dice.roll();
}

Game::Game()
{
	Player p1, p2;
	cout << "To start the game enter \"roll dice\"" << endl;
		showDices(p1);

}