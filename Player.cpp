#include "Player.h"

void Player::Dice::roll()
{
	m_top = 1 + (rand() % 6);
}

void Player::unkeepAll()
{
	for (int i = 0; i < m_dices.size(); i++)
		m_dices[i].unkeep();
}