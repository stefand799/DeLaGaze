#include "Player.h"

// Constructors
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

// Getteri
const int Player::GetId() const{ return m_id; }
const std::string Player::GetUsername() const { return m_username; }
uint8_t Player::GetScore() const { return m_score; }
uint16_t Player::GetPoints() const { return m_points; }
uint8_t Player::GetFireRate() const { return m_bulletSpeed; }
const bool Player::GetBulletSpeedUpgrade() const { return m_bulletSpeedUpgrade; }
uint8_t Player::GetHp() const{ return m_hp; }
int Player::GetX() const { return m_x; }
int Player::GetY() const { return m_y; }
State Player::GetPlayerState() const { return m_playerState; }
Direction Player::GetFacing() const { return m_facing; }
Map* Player::GetMap() { return m_playerMap; }

// Setteri
void Player::SetId(int id) { m_id = id; }
void Player::SetUsername(const std::string& username) { m_username = username; }
void Player::SetScore(uint8_t score) { m_score = score; }
void Player::SetPoints(uint16_t points) { m_points = points; }
void Player::SetFireRate(uint8_t bulletSpeed) { m_bulletSpeed = bulletSpeed; }
void Player::SetBulletSpeedUpgrade(bool bulletSpeedUpgrade) { m_bulletSpeedUpgrade = bulletSpeedUpgrade; }
void Player::SetHp() { m_hp = GetHp() - 1; }
void Player::SetFacing(const Direction& facing) { m_facing = facing; }
void Player::SetPlayerState(const State& playerState) { m_playerState = playerState; }
void Player::SetX(const int& x) { m_x = x; }
void Player::SetY(const int& y) { m_y = y; }

// Movement
bool Player::CanMoveHere(int i, int j) { return false; }
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

// Facing
void Player::FaceNorth() { m_facing = Direction::North; }
void Player::FaceSouth() { m_facing = Direction::South; }
void Player::FaceWest() { m_facing = Direction::West; }
void Player::FaceEast() { m_facing = Direction::East; }

// Functionalities
void Player::Shoot(std::vector<Bullet*>& bullets) {
	uint8_t bulletSpeed = m_bulletSpeedUpgrade ? kBulletSpeeds[1] : kBulletSpeeds[0]; //Placeholder for bullet speeds
	bullets.emplace_back(new Bullet(m_x, m_y, bulletSpeed, m_facing)); // set the position the bullet a little in the direction of the player facing
	//bullets.push_back(std::make_unique<Bullet>(m_x, m_y, bulletSpeed, m_facing)); // set the position the bullet a little in the direction of the player facing
};
void Player::Respawn() {
	SetX(m_spawnpoint.first);
	SetY(m_spawnpoint.second);
}

void Player::Render()
{

}
void Player::Print() const
{
	if (m_facing == Direction::North) std::cout << "\033[37;41m" << "^" << "\033[0m";
	if (m_facing == Direction::South) std::cout << "\033[37;41m" << "v" << "\033[0m";
	if (m_facing == Direction::West) std::cout << "\033[37;41m" << "<" << "\033[0m";
	if (m_facing == Direction::East) std::cout << "\033[37;41m" << ">" << "\033[0m";
}
std::string Player::StateToString(State state)
{
	switch (state)
	{
	case State::Idle: return "Idle";
	case State::Moving: return "Moving";
	case State::Shooting: return "Shooting";
	default: return "Unknown";

	}
}
crow::json::wvalue Player::BulletsToJson()
{
	std::vector<crow::json::wvalue> jsonBullets;
	for (const auto& bullet : m_bullets)
	{
		jsonBullets.push_back(bullet->toJson());
	}
	return jsonBullets;
}
std::string Player::DirectionToString(Direction direction)
{
	switch (direction) {
	case Direction::North: return "North";
	case Direction::East: return "East";
	case Direction::South: return "South";
	case Direction::West: return "West";
	default: return "Unknown";
	}
}
crow::json::wvalue Player::toJson()
{

	crow::json::wvalue jsonPlayerObj;
	jsonPlayerObj["type"] = "Player";
	jsonPlayerObj["id"] = m_id;
	jsonPlayerObj["username"] = m_username;
	jsonPlayerObj["x"] = m_x;
	jsonPlayerObj["y"] = m_y;
	jsonPlayerObj["facing"] = DirectionToString(m_facing);
	jsonPlayerObj["state"] = StateToString(m_playerState);
	jsonPlayerObj["spawnX"] = m_spawnpoint.first;
	jsonPlayerObj["spawnY"] = m_spawnpoint.second;
	return jsonPlayerObj;
}
