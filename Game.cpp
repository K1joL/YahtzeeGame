#include "Game.h"
#include <iostream>

using namespace std;

void Game::showDices(Player &p)
{
	int index = 1;
	for (auto &dice : p.getDices())
	{
		cout << index << " Dice - " << dice.getTop();
		if (dice.isKeeped())
			cout << "keeped";
		cout << endl;
		index++;
	}
}

Game::Game()
{
	Player p1, p2;
	cout << "To start the game enter \"roll dice\"" << endl;
	showDices(p1);

}