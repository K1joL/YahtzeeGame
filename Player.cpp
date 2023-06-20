#include "Player.h"

void Player::Dice::roll()
{
	m_top = 1 + (rand() % 6);
}

void Player::_createTable()
{
	m_scoreTable.emplace("O", Score());
	m_scoreTable.emplace("Tw", Score());
	m_scoreTable.emplace("Th", Score());
	m_scoreTable.emplace("Fr", Score());
	m_scoreTable.emplace("Fv", Score());
	m_scoreTable.emplace("S", Score());
	m_scoreTable.emplace("Tk", Score());
	m_scoreTable.emplace("Fk", Score());
	m_scoreTable.emplace("Fh", Score());
	m_scoreTable.emplace("Ss", Score());
	m_scoreTable.emplace("Ls", Score());
	m_scoreTable.emplace("CH", Score());
	m_scoreTable.emplace("Y", Score());
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
