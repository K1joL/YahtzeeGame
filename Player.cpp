#include "Player.h"

void Player::Dice::roll()
{
	m_top = 1 + (rand() % 6);
}
