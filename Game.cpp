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

bool sortByScores(const Player& left, const Player& right)
{
	return left.getScores() > right.getScores();
}

void Game::initComboMap()
{
	m_comboMap.emplace("O", Ones);
	m_comboMap.emplace("Tw",Twos );
	m_comboMap.emplace("Th", Threes);
	m_comboMap.emplace("Fr", Fours);
	m_comboMap.emplace("Fv", Fives);
	m_comboMap.emplace("S", Sixes);
	m_comboMap.emplace("Tk", ThreeOfAKind);
	m_comboMap.emplace("Fk", FourOfAKind);
	m_comboMap.emplace("Fh", FullHouse);
	m_comboMap.emplace("Ss", SmallStraight);
	m_comboMap.emplace("Ls", LargeStraight);
	m_comboMap.emplace("CH", Chance);
	m_comboMap.emplace("Y", Yahtzee);
}

void Game::startGame()
{
	std::vector<Player> playerQueue;
	cout << "Enter the number of players: " << endl;
	int numberOfPlayers = 0;
	cin >> numberOfPlayers;

	for (int i = 0; i < numberOfPlayers; i++)
		playerQueue.push_back(Player());
	for (int round = 0; round < 13; round++)
	{
		for (int i = 0; i < playerQueue.size(); i++)
		{
			Player& currentPlayer = playerQueue[i];
			cout << "Player " << i + 1 << " your turn." << endl;
			int rollNum = 0;
			while (rollNum < 4)
			{
				char action = ' ';
				while (action != 'R' && action != 'K' && action != 'T' && action != 'U')
				{
					cout << "You can:" << endl;
					if (rollNum < 3)
						cout << "(R)oll" << endl;
					if (rollNum != 0)
						cout << "(T)ake a combo" << endl;
					if (rollNum != 0)
						cout << "(K)eep the dice" << endl;
					if (currentPlayer.isOneKeeped())
						cout << "(U)nkeep the dice" << endl;
					cin >> action;
					string diceNumber = "";
					string combo = "";
					switch (action)
					{
					case 'R':
						rollDices(currentPlayer);
						rollNum++;
						break;

					case 'K':
						cout << "Enter the dice numbers you want to keep: ";
						cin >> diceNumber;
						keepDice(currentPlayer, diceNumber);
						break;

					case 'T':
						showCombos();
						cin >> combo;
						if (isValidCombo(currentPlayer, combo))
						{
							if (isYahtzee(currentPlayer))
								takeCombo(currentPlayer, combo, true);
							else
								takeCombo(currentPlayer, combo);
						}
						else
						{
							cout << "Something went wrong!" << endl;
							cout << "Make sure that you can do this!" << endl;
							break;
						}

						rollNum = 4; //End of turn
						break;
					case 'U':
					{
						char option;
						do
						{
							cout << "Unkeep (A)ll dices!" << endl
								<< "Unkeep (C)hosen dices!" << endl;
							cin >> option;
							switch (option)
							{
							case 'A':
								unkeepAll(currentPlayer);
								break;
							case 'C':
								cin >> diceNumber;
								unkeepDice(currentPlayer, diceNumber);
								break;
							default:
								cout << "Try again!" << endl;

							}
						} while (option != 'A' && option != 'C');
						break;
					}
					default:
						cout << "Try again!" << endl;
					}
					showScores(playerQueue);
					showDices(currentPlayer);
				}
			}
			unkeepAll(currentPlayer);
		}
	}
	showScores(playerQueue);
	whoWins(playerQueue);
}

void Game::showScores(const std::vector<Player> &pQueue) const
{
	for (int i = 0; i < pQueue.size(); i++)
		cout << "Player " << i + 1 << ": " << pQueue[i].getScores() << endl;
}

void Game::whoWins(const std::vector<Player>& pQueue) const
{
	const auto &winner = max_element(pQueue.begin(), pQueue.end(), sortByScores);
	cout << "Player " << winner->getID() << " wins with scores: " << winner->getScores() << "!" << endl;
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
	auto &table = p.getScoresTable();
	if (table.contains(combo))
		if(!table.at(combo).isFilled)
			return true;
		else if (combo == "Y")
			return p.isUpperSectionFilled();

	return false;
}

void Game::takeCombo(Player &p, std::string& combo, bool joker)
{
	Combo cmb = m_comboMap.at(combo);
	auto diceCopy = p.getDices();
	auto &dice = p.getDices();
	int scores = 0;
	if (cmb < 6)
	{
		scores = firstSectionSum(p, cmb + 1);
		p.earnScores(scores, combo);
		return;
	}
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
		if (joker == true)
		{
			scores = 25;
			break;
		}
		for (int i = 1; i <= 6; i++)
			if (std::count(dice.cbegin(), dice.cend(), i) == 3)
			{
				for (int j = 1; j <= 6; j++)
					if (i != j && std::count(dice.cbegin(), dice.cend(), j) == 2)
					{
						scores = 25;
						break;
					}
				break;
			}

		break;
	case SmallStraight:
	{
		if (joker == true)
		{
			scores = 30;
			break;
		}
		sort(diceCopy.begin(), diceCopy.end());
		auto last = std::unique(diceCopy.begin(), diceCopy.end());
		diceCopy.erase(last, diceCopy.end());
		for (int i = 0; i < 2; i++)
		{
			int j = i;
			while (j < 3 && (diceCopy[j + 1].getTop() - diceCopy[j].getTop()) == 1)
				j++;
			if (j == 3)
				scores = 30;
			i++;
		}
		break;
	}
	case LargeStraight:
	{
		if (joker == true)
		{
			scores = 40;
			break;
		}
		sort(diceCopy.begin(), diceCopy.end());
		auto last = std::unique(diceCopy.begin(), diceCopy.end());
		diceCopy.erase(last, diceCopy.end());
		for (int i = 0; i < 1; i++)
		{
			int j = i;
			while (j < 4 && (diceCopy[j + 1].getTop() - diceCopy[j].getTop()) == 1)
				j++;
			if (j == 4)
				scores = 40;
			i++;
		}
		break;
	}
	case Chance:
		scores = sumAllDice(p);
		break;
	case Yahtzee:
		if(!p.getScoresTable().at(combo).isFilled)
		{
			if (isYahtzee(p))
				scores = 50;
		}
		else if (isYahtzee(p))
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
	return (count(dice.cbegin(), dice.cend(), dice[0].getTop()) == 5);
}