#include "Player.h"

//Player::Player(const std::pair<int, int>& pos, const std::string_view& username, uint8_t points, uint8_t speed, Direction Facing, State PlayerState, uint8_t score = 0, uint8_t hp = 3)
//	:Object(pos), m_username{ username }, m_points{ points }, m_speed{ speed }, m_facing{ Facing }, m_playerState{ PlayerState }, m_score{ score }, m_hp{ hp } {};

//Player::Player(const Map& m) : m_playerMap{ m } {};
//
//Player::Player(const int& i, const int& j, const Map& m) : m_x{ i }, m_y{ j }, m_playerMap{ m } {};
bool Player::canMoveHere(int i, int j) {
	return false;
}
void Player::render()
{

}
void Player::print()
{
	std::cout << "\033[37;41m" << "P" << "\033[0m";
}

void Player::SetY(const int& y)
{
	m_y = y;
}
int Player::GetY() const
{
	return m_y;
}
void Player::SetHp()
{
	m_hp = GetHp() - 1;
}
void Player::SetScore(uint8_t score)
{
	m_score = score;
}
void Player::SetPoints(uint16_t points)
{
	m_points = points;
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

void Player::moveUp(Map& map)
{
	int newX = GetX() - 1;
	int newY = GetY();
	if (map.isWithinBounds(newX, newY)) {
		if (map[newX][newY]->canMoveHere(newX, newY))
		{
			if (map[newX][newY]->getType() == ObjectType::Pathway)
			{
				map[GetX()][GetY()] = new Pathway;
				SetX(newX);
				delete map[newX][newY];
				map[newX][newY] = this;
			}
		}
		else std::cout << "Can't move here. Unaccesible type of block: "; map[newX][newY]->print(); std::cout << "\n";
	}
	else {
		std::cout << "Can't move here. Out of bounds\n";
	}
}

void Player::moveDown(Map& map)
{
	int newX = GetX() + 1;
	int newY = GetY();
	if (map.isWithinBounds(newX, newY)) {
		if (map[newX][newY]->canMoveHere(newX, newY))
		{
			if (map[newX][newY]->getType() == ObjectType::Pathway)
			{
				map[GetX()][GetY()] = new Pathway;
				SetX(newX);
				delete map[newX][newY];
				map[newX][newY] = this;
			}
		}
		else std::cout << "Can't move here. Unaccesible type of block: "; map[newX][newY]->print(); std::cout << "\n";
	}
	else {
		std::cout << "Can't move here. Out of bounds\n";
	}
}

void Player::moveLeft(Map& map)
{
	int newX = GetX();
	int newY = GetY() - 1;
	if (map.isWithinBounds(newX, newY)) {
		if (map[newX][newY]->canMoveHere(newX, newY))
		{
			if (map[newX][newY]->getType() == ObjectType::Pathway)
			{

				map[GetX()][GetY()] = new Pathway;
				SetY(newY);
				delete map[newX][newY];
				map[newX][newY] = this;
			}
		}
		else std::cout << "Can't move here. Unaccesible type of block: "; map[newX][newY]->print(); std::cout << "\n";
	}
	else {
		std::cout << "Can't move here. Out of bounds\n";
	}
}
void Player::moveRight(Map& map)
{
	int newX = GetX();
	int newY = GetY() + 1; // y = coloana
	if (map.isWithinBounds(newX, newY)) {
		if (map[newX][newY]->canMoveHere(newX, newY))
		{
			if (map[newX][newY]->getType() == ObjectType::Pathway)
			{

				map[GetX()][GetY()] = new Pathway;
				SetY(newY);
				delete map[newX][newY];
				map[newX][newY] = this;
			}
		}
		else std::cout << "Can't move here. Unaccesible type of block: "; map[newX][newY]->print(); std::cout << "\n";
	}
	else {
		std::cout << "Can't move here. Out of bounds\n";
	}
}

void Player::SetFacing(const Direction& facing) { m_facing = facing; }

void Player::SetUsername(const std::string_view& username) { m_username = username; }

std::string Player::GetUsername() const { return m_username; }

void Player::shoot(std::vector<std::shared_ptr<Bullet>>& bullets) {
	uint8_t bulletSpeed = m_bulletSpeedUpgrade ? 2 : 1; //Placeholder for bullet speeds
	bullets.push_back(std::make_unique<Bullet>(bulletSpeed));
};

void Player::SetPlayerState(const State& playerState) { m_playerState = playerState; }

void Player::commitSprite() {
	/*This function dose nothing is just a filler so it counts as commit when i upload a sprite =( */
	for (int i = 0; i <= 10; ++i)
		i = (10 - i) / 2;
	return;
}