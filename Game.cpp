#include "Game.h"

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

void Game::showCombos()
{
	cout << setw(22) << left << "Uppper section combos:" << "Lower section combos:" << endl;
	cout << setw(22) << left << "Ones (O)"	<< "Three of a kind (Tk)" << endl
		<< setw(22) << left << "Twos (Tw)"	<< "Four of a kind (Fk)" << endl
		<< setw(22) << left << "Three (Th)" << "Full house (Fh)" << endl
		<< setw(22) << left << "Fours (Fr)" << "Small straight (Ss)" << endl
		<< setw(22) << left << "Fives (Fv)" << "Large straight (Ls)" << endl
		<< setw(22) << left << "Sixes (S)"	<< "Chance (Ch)" << endl
						<< setw(22) << left << "YAHTZEE (Y)" << endl;

}
