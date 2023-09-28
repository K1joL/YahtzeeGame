#include "Player.h"

void Player::Dice::roll()
{
	m_top = 1 + (rand() % 6);
}

void Player::_createTable()
{
	m_scoreTable.emplace("Ones", Score());
	m_scoreTable.emplace("Twos", Score());
	m_scoreTable.emplace("Threes", Score());
	m_scoreTable.emplace("Fours", Score());
	m_scoreTable.emplace("Fives", Score());
	m_scoreTable.emplace("Sixes", Score());
	m_scoreTable.emplace("Three Of A Kind", Score());
	m_scoreTable.emplace("Four Of A Kind", Score());
	m_scoreTable.emplace("Full House", Score());
	m_scoreTable.emplace("Small Straight", Score());
	m_scoreTable.emplace("Large Straight", Score());
	m_scoreTable.emplace("Chance", Score());
	m_scoreTable.emplace("Yahtzee", Score());
}

void Player::earnScores(int scores, std::string combo)
{
	m_score += scores;
	m_scoreTable.at(combo).score = scores;
	m_scoreTable.at(combo).isFilled = true;
}

bool Player::isUpperSectionFilled() const
{
	int i = 0;
	for (auto& dice : m_scoreTable)
		if (dice.second.isFilled && dice.first != "Tk")
			i++;
	return (i == 6);
}

bool Player::isOneKeeped()
{
	for (auto& dice : m_dices)
		if (dice.isKeeped())
			return true;
	return false;
}

int Player::m_id = 0;