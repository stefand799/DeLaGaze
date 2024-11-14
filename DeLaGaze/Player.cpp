#include "Player.h"

//Player::Player(const std::pair<int, int>& pos, const std::string_view& username, uint8_t points, uint8_t speed, Direction Facing, State PlayerState, uint8_t score = 0, uint8_t hp = 3)
//	:Object(pos), m_username{ username }, m_points{ points }, m_speed{ speed }, m_facing{ Facing }, m_playerState{ PlayerState }, m_score{ score }, m_hp{ hp } {};

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
Direction Player::GetFacing() const
{
	return m_facing;
}
State Player::GetPlayerState() const
{
	return m_playerState;
}

void Player::moveUp(Map& map, int i, int j)
{
	if (map[i][j]->canMoveHere(i,j)) //on client side(QT), we will check if the future position of the player isWithinBounds() before calling any move<Direction> function
	{
		if (map[i][j]->getType() == ObjectType::Pathway)
		{
			map[GetX()][GetY()] = new Pathway;
			SetY(GetY() - 1);
			map[i][j] = this;
		}
	}
}

void Player::SetFacing(const Direction& facing) { m_facing = facing; }

void Player::SetUsername(const std::string_view& username) { m_username = username; }

std::string Player::GetUsername() const { return m_username; }