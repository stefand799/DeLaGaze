#include "Player.h"

// Constructors

Player::Player(Map* m, const std::pair<int, int>& pos, const int& id, const std::string& username, uint8_t points, uint8_t fireRate, bool bulletSpeedUpgrade, Direction facing, State playerState, uint8_t score, uint8_t hp) :
	m_playerMap{ m },
	m_username{ username },
	m_id{ id },
	m_points{ points },
	m_fireRate{ fireRate },
	m_bulletSpeedUpgrade{ bulletSpeedUpgrade },
	m_facing{ facing },
	m_score{ score },
	m_hp{ hp },
	m_mapX{ pos.first },
	m_mapY{ pos.second },
	m_previousMapX{ pos.first },
	m_previousMapY{ pos.second },
	m_x{ pos.first + 0.5f },
	m_y{ pos.second + 0.5f },
	m_spawnpoint{ pos },
	m_moveCooldown{ kDefaultMoveCooldown },
	m_playerSpeed{ 1.0f / m_moveCooldown },
	m_xSpeed{ 0.0f },
	m_ySpeed{ 0.0f },
	m_isMoving{ false },
	m_endOfMove{ Clock::now() },
	m_lastShotTime{ Clock::now() }
{}


// Getteri
const int Player::GetId() const{ return m_id; }
const std::string Player::GetUsername() const { return m_username; }
uint8_t Player::GetScore() const { return m_score; }
uint16_t Player::GetPoints() const { return m_points; }
uint8_t Player::GetFireRate() const { return m_bulletSpeed; }
const bool Player::GetBulletSpeedUpgrade() const { return m_bulletSpeedUpgrade; }
uint8_t Player::GetHp() const{ return m_hp; }
float Player::GetX() const { return m_x; }
float Player::GetY() const { return m_y; }
bool Player::GetMovingState() const { return m_isMoving; }
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
void Player::SetX(const int& x) { m_mapX = x; }
void Player::SetY(const int& y) { m_mapY = y; }

// Movement
bool Player::CanMoveHere(int i, int j) { return false; }

void Player::Move(float deltaTime)
{
	if (Clock::now() >= m_endOfMove) {
		m_isMoving = false;
		m_x = m_mapX + 0.5f;
		m_y = m_mapY + 0.5f;
		m_previousMapX = m_mapX;
		m_previousMapY = m_mapY;
		m_xSpeed = 0.0f;
		m_ySpeed = 0.0f;
		return;
	}
	if (m_previousMapX < m_mapX) {
		m_x = std::min(m_x+ deltaTime * m_playerSpeed, m_mapX + 0.5f);
	}
	if (m_previousMapX > m_mapX) {
		m_x = std::max(m_x - deltaTime * m_playerSpeed, m_mapX + 0.5f);
	}
	if (m_previousMapY < m_mapY) {
		m_y = std::min(m_y + deltaTime * m_playerSpeed, m_mapY + 0.5f);
	}
	if (m_previousMapY > m_mapY) {
		m_y = std::max(m_y - deltaTime * m_playerSpeed, m_mapY + 0.5f);;
	}
}

void Player::MoveUp()
{
	if (Clock::now()<m_endOfMove) return;
	int newX = m_mapX;
	int newY = m_mapY - 1;
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
		(*m_playerMap)[m_mapY][m_mapX] = new Pathway{ {m_mapY,m_mapX} };
		m_mapY = newY;
	}
	m_ySpeed = -m_playerSpeed;
	m_isMoving = true;
	m_endOfMove = Clock::now() + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(kDefaultMoveCooldown));
}
void Player::MoveDown()
{
	if (Clock::now() < m_endOfMove) return;
	int newX = m_mapX;
	int newY = m_mapY + 1;
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
		(*m_playerMap)[m_mapY][m_mapX] = new Pathway{ {m_mapY,m_mapX} };
		m_mapY = newY;
	}
	m_ySpeed = m_playerSpeed;
	m_isMoving = true;
	m_endOfMove = Clock::now() + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(kDefaultMoveCooldown));
}
void Player::MoveLeft()
{
	if (Clock::now() < m_endOfMove) return;
	int newX = m_mapX - 1;
	int newY = m_mapY;
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
		(*m_playerMap)[m_mapY][m_mapX] = new Pathway{ {m_mapY,m_mapX} };
		m_mapX = newX;
	}
	m_xSpeed = -m_playerSpeed;
	m_isMoving = true;
	m_endOfMove = Clock::now() + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(kDefaultMoveCooldown));
}
void Player::MoveRight()
{
	if (Clock::now() < m_endOfMove) return;
	int newX = m_mapX + 1;
	int newY = m_mapY;
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
		(*m_playerMap)[m_mapY][m_mapX] = new Pathway{ {m_mapY,m_mapX} };
		m_mapX = newX;
	}
	m_xSpeed = m_playerSpeed;
	m_isMoving = true;
	m_endOfMove = Clock::now() + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(kDefaultMoveCooldown));
}

// Facing
void Player::FaceNorth() { m_facing = Direction::North; }
void Player::FaceSouth() { m_facing = Direction::South; }
void Player::FaceWest() { m_facing = Direction::West; }
void Player::FaceEast() { m_facing = Direction::East; }

// Functionalities
void Player::Shoot(std::vector<Bullet*>& bullets) {
	if (fSecDur(Clock::now() - m_lastShotTime).count() < kFireRates[m_fireRate]) return;
	float bulletSpeed = kBulletSpeeds[m_bulletSpeedUpgrade]; //Placeholder for bullet speeds
	//uint8_t bulletSpeed = m_bulletSpeedUpgrade ? kBulletSpeeds[1] : kBulletSpeeds[0]; //Placeholder for bullet speeds
	switch (m_facing)
	{
	case Direction::North:
		bulletSpeed -= m_ySpeed < 0 ? m_ySpeed : 0.0f;
		break;
	case Direction::East:
		bulletSpeed += m_xSpeed > 0 ? m_xSpeed : 0.0f;
		break;
	case Direction::South:
		bulletSpeed += m_ySpeed > 0 ? m_ySpeed : 0.0f;
		break;
	case Direction::West:
		bulletSpeed -= m_xSpeed < 0 ? m_xSpeed : 0.0f;
		break;
	default:
		break;
	}
	bullets.emplace_back(new Bullet(m_x, m_y, bulletSpeed, m_facing)); // set the position the bullet a little in the direction of the player facing
	//bullets.push_back(std::make_unique<Bullet>(m_x, m_y, bulletSpeed, m_facing)); // set the position the bullet a little in the direction of the player facing
	m_lastShotTime = Clock::now();
}
void Player::OnDeath()
{
	m_hp--;
	if (m_hp) {
		Respawn();
	}
	else {
		//TODO: Delete the player from the game
	}
}
;
void Player::Respawn() {
	(*m_playerMap)[m_mapY][m_mapX] = new Pathway{ {m_mapX,m_mapY} };
	m_mapX = m_spawnpoint.first;
	m_mapY = m_spawnpoint.second;
	//Check if there is a player or possition is accessible
	delete (*m_playerMap)[m_mapY][m_mapX];
	(*m_playerMap)[m_mapY][m_mapX] = this;
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
	jsonPlayerObj["x"] = m_mapX;
	jsonPlayerObj["y"] = m_mapY;
	jsonPlayerObj["facing"] = DirectionToString(m_facing);
	jsonPlayerObj["state"] = StateToString(m_playerState);
	jsonPlayerObj["spawnX"] = m_spawnpoint.first;
	jsonPlayerObj["spawnY"] = m_spawnpoint.second;
	return jsonPlayerObj;
}
