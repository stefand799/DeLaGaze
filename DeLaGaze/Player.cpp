#include "Player.h"


Player::Player(Map* m) : m_playerMap{ m } {} //delete this later, only for early on testing. Leave only the constructor with all parameters.


Player::Player(Map* m, const std::pair<int, int>& pos, const int& id, const std::string& username, uint8_t points, bool bulletSpeedUpgrade, Direction facing, State playerState, uint8_t score, uint8_t hp) :
	m_playerMap{ m },
	m_username{ username },
	m_id{ id },
	m_points{ points },
	m_bulletSpeedUpgrade{ bulletSpeedUpgrade },
	m_facing{ facing },
	m_score{ score },
	m_hp{ hp },
	m_x{ pos.first },
	m_y{ pos.second },
	m_moveCooldown{ kDefaultMoveCooldown },
	m_lastMovedTime{ Clock::now() },
	m_lastShotTime{ Clock::now() }
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
void Player::Print() const
{
	if(m_facing == Direction::North) std::cout << "\033[37;41m" << "^" << "\033[0m";
	if(m_facing == Direction::South) std::cout << "\033[37;41m" << "v" << "\033[0m";
	if(m_facing == Direction::West) std::cout << "\033[37;41m" << "<" << "\033[0m";
	if(m_facing == Direction::East) std::cout << "\033[37;41m" << ">" << "\033[0m";
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
	if (std::chrono::duration<float>(Clock::now() - m_lastMovedTime).count() < m_moveCooldown) return;
	int newX = m_x;
	int newY = m_y - 1;
	if (!m_playerMap->IsWithinBounds(newY, newX)) {
		return;
	}

	if (!(*m_playerMap)[newY][newX]->CanMoveHere(newY, newX)) {
		return;
	}

	if ((*m_playerMap)[newY][newX]->GetType() == ObjectType::Pathway)
	{
		delete (*m_playerMap)[newY][newX];
		(*m_playerMap)[newY][newX] = this;
		(*m_playerMap)[m_y][m_x] = new Pathway;
		SetY(newY);
	}
	m_lastMovedTime = Clock::now();
}

void Player::MoveDown()
{
	if (std::chrono::duration<float>(Clock::now() - m_lastMovedTime).count() < m_moveCooldown) return;
	int newX = m_x;
	int newY = m_y + 1;
	if (!(*m_playerMap).IsWithinBounds(newY, newX)) {
		return;
	}

	if (!(*m_playerMap)[newY][newX]->CanMoveHere(newY, newX)) {
		return;
	}

	if ((*m_playerMap)[newY][newX]->GetType() == ObjectType::Pathway)
	{
		delete (*m_playerMap)[newY][newX];
		(*m_playerMap)[newY][newX] = this;
		(*m_playerMap)[m_y][m_x] = new Pathway;
		SetY(newY);
	}
	m_lastMovedTime = Clock::now();
}

void Player::MoveLeft()
{
	if (std::chrono::duration<float>(Clock::now() - m_lastMovedTime).count() < m_moveCooldown) return;
	int newX = m_x - 1;
	int newY = m_y;
	if (!(*m_playerMap).IsWithinBounds(newY, newX)) {
		return;
	}

	if (!(*m_playerMap)[newY][newX]->CanMoveHere(newY, newX)) {
		return;
	}

	if ((*m_playerMap)[newY][newX]->GetType() == ObjectType::Pathway)
	{
		delete (*m_playerMap)[newY][newX];
		(*m_playerMap)[newY][newX] = this;
		(*m_playerMap)[m_y][m_x] = new Pathway;
		SetX(newX);
	}
	m_lastMovedTime = Clock::now();
}
void Player::MoveRight()
{
	if (std::chrono::duration<float>(Clock::now() - m_lastMovedTime).count() < m_moveCooldown) return;
	int newX = m_x + 1;
	int newY = m_y;
	if (!(*m_playerMap).IsWithinBounds(newY, newX)) {
		return;
	}

	if (!(*m_playerMap)[newY][newX]->CanMoveHere(newY, newX)) {
		return;
	}

	if ((*m_playerMap)[newY][newX]->GetType() == ObjectType::Pathway)
	{
		delete (*m_playerMap)[newY][newX];
		(*m_playerMap)[newY][newX] = this;
		(*m_playerMap)[m_y][m_x] = new Pathway;
		SetX(newX);
	}
	m_lastMovedTime = Clock::now();
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

void Player::Shoot(std::vector<Bullet*>& bullets) {
	uint8_t bulletSpeed = m_bulletSpeedUpgrade ? kBulletSpeeds[1] : kBulletSpeeds[0]; //Placeholder for bullet speeds
	bullets.emplace_back(new Bullet(m_x, m_y, bulletSpeed, m_facing)); // set the position the bullet a little in the direction of the player facing
	//bullets.push_back(std::make_unique<Bullet>(m_x, m_y, bulletSpeed, m_facing)); // set the position the bullet a little in the direction of the player facing
};

void Player::SetPlayerState(const State& playerState) { m_playerState = playerState; }

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

