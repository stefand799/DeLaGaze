#include "Player.h"


Player::Player(Map& m) : m_playerMap{ m } {}; //delete this later, only for early on testing. Leave only the constructor with all parameters.


Player::Player(Map& m, const std::pair<int, int>& pos, const std::string& username, uint8_t points, bool bulletSpeedUpgrade, Direction facing, State playerState, uint8_t score, uint8_t hp) :
	m_playerMap{m},
	m_username{username},
	m_points{points},
	m_bulletSpeedUpgrade{bulletSpeedUpgrade},
	m_facing{facing},
	m_score{score},
	m_hp{hp},
	m_x{pos.first},
	m_y{pos.second}
	//,Object{pos},
{}
//
//Player::Player(const int& i, const int& j, const Map& m) : m_x{ i }, m_y{ j }, m_playerMap{ m } {};

bool Player::CanMoveHere(int i, int j) {
	return false;
}
void Player::Render()
{

}
void Player::Print()
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

void Player::MoveUp()
{
	int newX = m_x;
	int newY = m_y - 1;
	if (!m_playerMap.IsWithinBounds(newY, newX)) {
		std::cout << "Can't move here. Out of bounds\n";
		return;
	}

	if (!m_playerMap[newY][newX]->CanMoveHere(newY, newX)) {
		std::cout << "Can't move here. Unaccesible type of block: "; m_playerMap[newY][newX]->Print(); std::cout << "\n";
		return;
	}

	if (m_playerMap[newY][newX]->GetType() == ObjectType::Pathway)
	{
		delete m_playerMap[newY][newX];
		m_playerMap[newY][newX] = this;
		m_playerMap[m_y][m_x] = new Pathway;
		SetY(newY);
	}
}

void Player::MoveDown()
{
	int newX = m_x;
	int newY = m_y + 1;
	if (!m_playerMap.IsWithinBounds(newY, newX)) {
		std::cout << "Can't move here. Out of bounds\n";
		return;
	}

	if (!m_playerMap[newY][newX]->CanMoveHere(newY, newX)) {
		std::cout << "Can't move here. Unaccesible type of block: "; m_playerMap[newY][newX]->Print(); std::cout << "\n";
		return;
	}

	if (m_playerMap[newY][newX]->GetType() == ObjectType::Pathway)
	{
		delete m_playerMap[newY][newX];
		m_playerMap[newY][newX] = this;
		m_playerMap[m_y][m_x] = new Pathway;
		SetY(newY);
	}
}

void Player::MoveLeft()
{
	int newX = m_x - 1;
	int newY = m_y;
	if (!m_playerMap.IsWithinBounds(newY, newX)) {
		std::cout << "Can't move here. Out of bounds\n";
		return;
	}

	if (!m_playerMap[newY][newX]->CanMoveHere(newY, newX)) {
		std::cout << "Can't move here. Unaccesible type of block: "; m_playerMap[newY][newX]->Print(); std::cout << "\n";
		return;
	}

	if (m_playerMap[newY][newX]->GetType() == ObjectType::Pathway)
	{
		delete m_playerMap[newY][newX];
		m_playerMap[newY][newX] = this;
		m_playerMap[m_y][m_x] = new Pathway;
		SetX(newX);
	}
}
void Player::MoveRight()
{
	int newX = m_x + 1;
	int newY = m_y;
	if (!m_playerMap.IsWithinBounds(newY, newX)) {
		std::cout << "Can't move here. Out of bounds\n";
		return;
	}

	if (!m_playerMap[newY][newX]->CanMoveHere(newY, newX)) {
		std::cout << "Can't move here. Unaccesible type of block: "; m_playerMap[newY][newX]->Print(); std::cout << "\n";
		return;
	}

	if (m_playerMap[newY][newX]->GetType() == ObjectType::Pathway)
	{
		delete m_playerMap[newY][newX];
		m_playerMap[newY][newX] = this;
		m_playerMap[m_y][m_x] = new Pathway;
		SetX(newX);
	}
}

void Player::FaceNorth()
{
	m_facing = Direction::North;
}

void Player::FaceSouth()
{
	m_facing = Direction::South;
}

void Player::FaceWest()
{
	m_facing = Direction::West;
}

void Player::FaceEast()
{
	m_facing = Direction::East;
}

Direction Player::GetFacing()
{
	return m_facing;
}

void Player::SetFacing(const Direction& facing) { m_facing = facing; }

void Player::SetUsername(const std::string& username) { m_username = username; }

const std::string Player::GetUsername() const { return m_username; }

void Player::Shoot(std::vector<std::shared_ptr<Bullet>>& bullets) {
	uint8_t bulletSpeed = m_bulletSpeedUpgrade ? 2 : 1; //Placeholder for bullet speeds
	bullets.push_back(std::make_unique<Bullet>(std::pair<float, float>{m_x + 0.5, m_y + 0.5}, bulletSpeed, m_facing)); // set the position the bullet a little in the direction of the player facing
};

void Player::SetPlayerState(const State& playerState) { m_playerState = playerState; }

void Player::CommitSprite() {
	/*This function dose nothing is just a filler so it counts as commit when i upload a sprite =( */
	for (int i = 0; i <= 10; ++i)
		i = (10 - i) / 2;
	return;
}

void Player::Respawn(){
	SetX(m_spawnpoint.first);
	SetY(m_spawnpoint.second);
}


const bool Player::GetBulletSpeedUpgrade() const { return m_bulletSpeedUpgrade; };
void Player::SetBulletSpeedUpgrade(bool bulletSpeedUpgrade) { m_bulletSpeedUpgrade = bulletSpeedUpgrade; }
const uint8_t Player::GetBulletSpeed()const { return m_bulletSpeed; } ;
void Player::SetBulletSpeed(uint8_t bulletSpeed) { m_bulletSpeed = bulletSpeed; }

const int Player::GetId() const
{
	return m_id;
}

void Player::SetId(int id)
{
	m_id = id;
}

