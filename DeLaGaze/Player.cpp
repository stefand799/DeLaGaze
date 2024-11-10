#include "Player.h"



bool Player::canMoveHere(int i,int j){
	return false;
}
void Player::render() 
{

}
void Player::print()
{
	std::cout << "P";
}

void Player::SetY(const int& y)
{
	m_y = y;
}
int Player::GetY() const
{
	return m_y;
}
uint8_t Player::GetHp() const
{
	return m_hp;
}
uint8_t Player::GetScore() const
{
	return m_score;
}
uint16_t Player::GetPoints() const
{
	return m_points;
}
void Player::SetX(const int& x)
{
	m_x = x;
}
int Player::GetX() const
{
	return m_x;
}

