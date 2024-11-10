#include "Player.h"

Player::Player()
{
	m_hp = 3;
	m_score = 0;
	m_points = 0;
}
Player::~Player()
{
}
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

void Player::SetY(int y)
{
	m_y = y;
}
int Player::GetY()
{
	return m_y;
}
uint8_t Player::GetHp()
{
	return m_hp;
}
uint8_t Player::GetScore()
{
	return m_score;
}
int Player::GetPoints()
{
	return m_points;
}
void Player::SetX(int x)
{
	m_x = x;
}
int Player::GetX()
{
	return m_x;
}

