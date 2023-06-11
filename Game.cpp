#include "Game.h"

using namespace std;


int sumAllDice(const Player& p)
{
	int sum = 0;
	for (auto& dice : p.getDices())
		sum += dice.getTop();
	return sum;
}

int firstSectionSum(const Player& p, int diceValue)
{
	int sum = 0;
	for (auto& dice : p.getDices())
	{
		if (dice.getTop() == diceValue)
			sum += diceValue;
	}
	return sum;
}



void Game::showDices(const Player &p) const
{
	int index = 0;
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

void Game::unkeepDice(Player& p, const std::string& diceNumber)
{
	for (int i = 0; i < diceNumber.length(); i++)
	{
		int diceIndex = diceNumber[i] - '0';
		p.getDices()[diceIndex].unkeep();
	}
}

void Game::unkeepAll(Player& p)
{
	for (auto& dice : p.getDices())
		dice.unkeep();
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
	cout << setw(22) << left << "Ones (O)"				 << "Three of a kind (Tk)" << endl
		 << setw(22) << left << "Twos (Tw)"				 << "Four of a kind (Fk)" << endl
		 << setw(22) << left << "Three (Th)"			 << "Full house (Fh)" << endl
		 << setw(22) << left << "Fours (Fr)"			 << "Small straight (Ss)" << endl
		 << setw(22) << left << "Fives (Fv)"			 << "Large straight (Ls)" << endl
		 << setw(22) << left << "Sixes (S)"				 << "Chance (Ch)" << endl
		 << setw(22) << left << " "						 << "YAHTZEE (Y)" << endl;
}

bool Game::isValidCombo(const Player& p, std::string combo)
{
	auto &table = p.getScores();
	if (table.contains(combo))
		if(!table.at(combo).isFilled)
			return true;
		else if (combo == "Y")
			return p.isUpperSectionFilled();

	return false;
}

void Game::takeCombo(Player &p, std::string& combo)
{
	Combo cmb = m_comboMap.at(combo);
	auto diceCopy = p.getDices();
	auto &dice = p.getDices();
	int scores = 0;
	if (cmb < 6)
		scores = firstSectionSum(p, cmb);
	switch (cmb)
	{
	case ThreeOfAKind:
		for (int i = 1; i <= 6; i++)
			if (std::count(dice.cbegin(), dice.cend(), i) >= 3)
				scores = sumAllDice(p);
		break;
	case FourOfAKind:
		for (int i = 1; i <= 6; i++)
			if (std::count(dice.cbegin(), dice.cend(), i) >= 4)
				scores = sumAllDice(p);
		break;
	case FullHouse:
		for (int i = 1; i <= 6; i++)
			if (std::count(dice.cbegin(), dice.cend(), i) == 3)
				for (int j = 1; j <= 6; j++)
					if (i != j && std::count(dice.cbegin(), dice.cend(), i) == 2)
						scores = 25;
		break;
	case SmallStraight:
		sort(diceCopy.begin(), diceCopy.end());
		for (int i = 0; i < 3; i++)
		{
			int j = i;
			while ((diceCopy[j + 1].getTop() - diceCopy[j].getTop()) == 1)
			{
				if (j < 3)
					j++;
				else break;
			}
			if (j == 3)
				scores = 30;
			i++;
		}
		break;

	case LargeStraight:
		sort(diceCopy.begin(), diceCopy.end());
		for (int i = 0; i < 3; i++)
		{
			int j = i;
			while ((diceCopy[j + 1].getTop() - diceCopy[j].getTop()) == 1)
			{
				if (j < 4)
					j++;
				else break;
			}
			if (j == 4)
				scores = 40;
			i++;
		}
		break;
	case Chance:
		scores = sumAllDice(p);
		break;
	case Yahtzee:
		if(!p.getScores().at(combo).isFilled)
		{
			if (isYahtzee(p))
				scores = 50;
		}
		else if (p.isUpperSectionFilled())
			if (isYahtzee(p))
				scores = 100;
		break;
	default:
		cout << "Try Again!" << endl;
	}
	p.earnScores(scores, combo);
}

bool Game::isYahtzee(Player& p)
{
	auto &dice = p.getDices();
	for (int i = 0; i < dice.size(); i++)
		return (count(dice.cbegin(), dice.cend(), dice[0].getTop()) == 5);
}
