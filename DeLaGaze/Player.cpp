#include "Player.h"
#include "Bullet.h"
#include "DeadlyBlock.h"

// Constructors

Player::Player(Map* m, const std::pair<int, int>& pos, const int& id, const std::string& username, uint8_t points, uint8_t fireRate, bool bulletSpeedUpgrade, Direction facing, State playerState, uint8_t score, uint8_t teamid, uint8_t hp) :
	m_playerMap{ m },
	m_username{ username },
	m_id{ id },
	m_points{ points },
	m_fireRate{ fireRate },
	m_bulletSpeedUpgrade{ bulletSpeedUpgrade },
	m_facing{ facing },
	m_score{ score },
	m_hp{ hp },
	m_teamId{ teamid },
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
	m_lastShotTime{ Clock::now() },
	m_lastDeathTime{ Clock::now() }
{}
Player::Player(const int& id, const std::string& username, uint8_t score, uint16_t points, uint8_t fireRate, bool bulletSpeedUpgrade)
	: m_id(id),
	m_username(username),
	m_score(score),
	m_points(points),
	m_bulletSpeed(fireRate),
	m_bulletSpeedUpgrade(bulletSpeedUpgrade) {}

// Operators

Player& Player::operator=(const Player& other) {
	if (this == &other)

	m_id = other.m_id;
	m_username = other.m_username;
	m_hp = other.m_hp;
	m_score = other.m_score;
	m_bulletSpeed = other.m_bulletSpeed;
	m_points = other.m_points;
	m_mapX = other.m_mapX;
	m_mapY = other.m_mapY;
	m_previousMapX = other.m_previousMapX;
	m_previousMapY = other.m_previousMapY;
	m_x = other.m_x;
	m_y = other.m_y;
	m_xSpeed = other.m_xSpeed;
	m_ySpeed = other.m_ySpeed;
	m_bulletSpeedUpgrade = other.m_bulletSpeedUpgrade;
	m_fireRate = other.m_fireRate;
	m_facing = other.m_facing;
	m_playerState = other.m_playerState;
	m_shootCooldown = other.m_shootCooldown;
	m_lastShotTime = other.m_lastShotTime;
	m_moveCooldown = other.m_moveCooldown;
	m_playerSpeed = other.m_playerSpeed;
	m_isMoving = other.m_isMoving;
	m_endOfMove = other.m_endOfMove;
	m_spawnpoint = other.m_spawnpoint;
	m_playerMap = other.m_playerMap;

	return *this;
}


// Getters
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
Map* Player::GetMap() const { return m_playerMap; }
uint8_t Player::GetTeamId() const { return m_teamId; }

// Setters
void Player::SetId(int id) { m_id = id; }
void Player::SetUsername(const std::string& username) { m_username = username; }
void Player::SetScore(uint8_t score) { m_score = score; }
void Player::SetPoints(uint16_t points) { m_points = points; }
void Player::SetFireRate(uint8_t bulletSpeed) { m_bulletSpeed = bulletSpeed; }
void Player::SetBulletSpeedUpgrade(bool bulletSpeedUpgrade) { m_bulletSpeedUpgrade = bulletSpeedUpgrade; }
void Player::SetHp(uint8_t hp) { m_hp = hp; }
void Player::SetFacing(const Direction& facing) { m_facing = facing; }
void Player::SetPlayerState(const State& playerState) { m_playerState = playerState; }
void Player::SetX(const int& x) { m_mapX = x; }
void Player::SetY(const int& y) { m_mapY = y; }

// Movement
bool Player::CanMoveHere() { return false; }

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
	this->SetPlayerState(State::Idle);
}

void Player::MoveUp()
{
	if (m_hp == 0) return;
	if (Clock::now()<m_endOfMove) return;
	int newX = m_mapX;
	int newY = m_mapY - 1;
	if (!m_playerMap->IsWithinBounds(newY, newX)) {
		return;
	}

	if (!(*m_playerMap)[newY][newX]->CanMoveHere()) {
		return;
	}

	if (std::shared_ptr<DeadlyBlock> deadlyBlock = std::dynamic_pointer_cast<DeadlyBlock>((*m_playerMap)[newY][newX])) {
		this->OnDeath();
		return;
	}

	if ((*m_playerMap)[newY][newX]->GetType() == ObjectType::Pathway)
	{
		(*m_playerMap)[newY][newX].swap((*m_playerMap)[m_mapY][m_mapX]);
		(*m_playerMap)[m_mapY][m_mapX] = std::make_shared<Pathway>(std::pair<size_t, size_t>{m_mapY, m_mapX});
		m_mapY = newY;
	}

	m_ySpeed = -m_playerSpeed;
	m_isMoving = true;
	m_endOfMove = Clock::now() + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(kDefaultMoveCooldown));
}
void Player::MoveDown()
{
	if (m_hp == 0) return;
	if (Clock::now() < m_endOfMove) return;
	int newX = m_mapX;
	int newY = m_mapY + 1;
	if (!(*m_playerMap).IsWithinBounds(newY, newX)) {
		return;
	}

	if (!(*m_playerMap)[newY][newX]->CanMoveHere()) {
		return;
	}

	if (std::shared_ptr<DeadlyBlock> deadlyBlock = std::dynamic_pointer_cast<DeadlyBlock>((*m_playerMap)[newY][newX])) {
		this->OnDeath();
		return;
	}

	if ((*m_playerMap)[newY][newX]->GetType() == ObjectType::Pathway)
	{
		(*m_playerMap)[newY][newX].swap((*m_playerMap)[m_mapY][m_mapX]);
		(*m_playerMap)[m_mapY][m_mapX] = std::make_shared<Pathway>(std::pair<size_t, size_t>{m_mapY, m_mapX});
		m_mapY = newY;
	}

	m_ySpeed = m_playerSpeed;
	m_isMoving = true;
	m_endOfMove = Clock::now() + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(kDefaultMoveCooldown));
}
void Player::MoveLeft()
{
	if (m_hp == 0) return;
	if (Clock::now() < m_endOfMove) return;
	int newX = m_mapX - 1;
	int newY = m_mapY;
	if (!(*m_playerMap).IsWithinBounds(newY, newX)) {
		return;
	}

	if (!(*m_playerMap)[newY][newX]->CanMoveHere()) {
		return;
	}

	if (std::shared_ptr<DeadlyBlock> deadlyBlock = std::dynamic_pointer_cast<DeadlyBlock>((*m_playerMap)[newY][newX])) {
		this->OnDeath();
		return;
	}

	if ((*m_playerMap)[newY][newX]->GetType() == ObjectType::Pathway)
	{
		(*m_playerMap)[newY][newX].swap((*m_playerMap)[m_mapY][m_mapX]);
		(*m_playerMap)[m_mapY][m_mapX] = std::make_shared<Pathway>(std::pair<size_t, size_t>{m_mapY, m_mapX});
		m_mapX = newX;
	}

	m_xSpeed = -m_playerSpeed;
	m_isMoving = true;
	m_endOfMove = Clock::now() + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(kDefaultMoveCooldown));
}
void Player::MoveRight()
{
	if (m_hp == 0) return;
	if (Clock::now() < m_endOfMove) return;
	int newX = m_mapX + 1;
	int newY = m_mapY;
	if (!(*m_playerMap).IsWithinBounds(newY, newX)) {
		return;
	}

	if (!(*m_playerMap)[newY][newX]->CanMoveHere()) {
		return;
	}

	if (std::shared_ptr<DeadlyBlock> deadlyBlock = std::dynamic_pointer_cast<DeadlyBlock>((*m_playerMap)[newY][newX])) {
		this->OnDeath();
		return;
	}

	if ((*m_playerMap)[newY][newX]->GetType() == ObjectType::Pathway)
	{
		(*m_playerMap)[newY][newX].swap((*m_playerMap)[m_mapY][m_mapX]);
		(*m_playerMap)[m_mapY][m_mapX] = std::make_shared<Pathway>(std::pair<size_t, size_t>{m_mapY, m_mapX});
		m_mapX = newX;
	}

	m_xSpeed = m_playerSpeed;
	m_isMoving = true;
	m_endOfMove = Clock::now() + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(kDefaultMoveCooldown));
}

// Facing
void Player::FaceNorth() { m_facing = Direction::North; m_playerState = State::Idle; }
void Player::FaceSouth() { m_facing = Direction::South; m_playerState = State::Idle;}
void Player::FaceWest() { m_facing = Direction::West; m_playerState = State::Idle;}
void Player::FaceEast() { m_facing = Direction::East; m_playerState = State::Idle;}

// Functionalities
void Player::Shoot(std::vector<std::shared_ptr<Bullet>>& bullets) {
	if (m_hp == 0) return;
	if (fSecDur(Clock::now() - m_lastShotTime).count() < kFireRates[m_fireRate]) return;
	float bulletSpeed = kBulletSpeeds[m_bulletSpeedUpgrade];
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
	bullets.emplace_back(std::move(std::make_shared<Bullet>(std::dynamic_pointer_cast<Player>((*m_playerMap)[m_mapY][m_mapX]) , m_x, m_y, bulletSpeed, m_facing)));
	m_lastShotTime = Clock::now();
	this->SetPlayerState(State::Shooting);
}
void Player::Kill()
{
	if (fSecDur(Clock::now() - m_lastDeathTime).count() < kSpawnProtection) return;
	OnDeath();
	m_lastDeathTime = Clock::now();
}
void Player::OnDeath()
{
	m_hp--;
	if (m_hp > 0) {
		Respawn();
	}
	else {
		//TODO: Delete the player from the game
		(*m_playerMap)[m_mapY][m_mapX] = std::make_shared<Pathway>(std::pair<size_t, size_t>{m_mapY, m_mapX});
	}
}
void Player::Respawn() {
	if (m_mapX != m_spawnpoint.first || m_mapY != m_spawnpoint.second)
	{
		if (std::shared_ptr<Player> otherPlayerOnSpawnpoint = std::dynamic_pointer_cast<Player>((*m_playerMap)[m_spawnpoint.second][m_spawnpoint.first])) {
			otherPlayerOnSpawnpoint->OnDeath();
		}
		(*m_playerMap)[m_mapY][m_mapX].swap((*m_playerMap)[m_spawnpoint.second][m_spawnpoint.first]);
		(*m_playerMap)[m_mapY][m_mapX] = std::make_shared<Pathway>(std::pair<size_t, size_t>{m_mapY, m_mapX});
		m_mapX = m_spawnpoint.first;
		m_mapY = m_spawnpoint.second;
	}
	m_x = m_mapX + 0.5f;
	m_y = m_mapY + 0.5f;
	m_endOfMove = Clock::now();
}
std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float> Player::GetBulletToPlayerColision(std::shared_ptr<Bullet> bullet)
{
	float bulletX = bullet->GetX();
	float bulletY = bullet->GetY();

	float bulletXSpeed = bullet->GetXSpeed();
	float bulletYSpeed = bullet->GetYSpeed();
	float bulletRadius = bullet->GetRadius();

	float collisionDistance = kPlayerHitBoxRadius + bulletRadius;

	float x12 = m_x - bulletX;
	float y12 = m_y - bulletY;

	float vx12 = m_xSpeed - bulletXSpeed;
	float vy12 = m_ySpeed - bulletYSpeed;

	float a = vx12 * vx12 + vy12 * vy12;
	float b = 2 * (x12 * vx12 + y12 * vy12);
	float c = x12 * x12 + y12 * y12 - collisionDistance * collisionDistance;
	if (c <= 0.0f) return std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float>{ (*m_playerMap)[m_mapY][m_mapX], bullet, 0.0f };

	float delta = (b * b) - (4 * a * c);

	if (delta < 0 || a == 0.0f) return std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float>{ (*m_playerMap)[m_mapY][m_mapX], bullet, NAN };

	float sqrtDelta = sqrt(delta);
	float t1 = (-b - sqrtDelta) / (2 * a);
	float t2 = (-b + sqrtDelta) / (2 * a);

	if (t1 < 0 && t2 < 0) return std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float>{ (*m_playerMap)[m_mapY][m_mapX], bullet, NAN };

	float T = 0.0f;

	if (t1 < 0) T = t2;
	else if (t2 < 0) T = t1;
	else T = std::min(t1, t2);

	if (m_isMoving && Clock::now() + fSecDur(T) > m_endOfMove) T = NAN;

	return std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float>{(*m_playerMap)[m_mapY][m_mapX], bullet, T };
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
	case State::Shooting: return "Shooting";
	default: return "Unknown";

	}
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
	jsonPlayerObj["teamId"] = m_teamId;
	jsonPlayerObj["x"] = m_x;
	jsonPlayerObj["y"] = m_y;
	jsonPlayerObj["facing"] = DirectionToString(m_facing);
	jsonPlayerObj["state"] = StateToString(m_playerState);
	jsonPlayerObj["spawnX"] = m_spawnpoint.first;
	jsonPlayerObj["spawnY"] = m_spawnpoint.second;
	return jsonPlayerObj;
}
void Player::SetPlayerInGame(Map* map, const std::pair<int, int>& pos, Direction facing, State state, uint8_t teamid, uint8_t hp) {
	
	m_playerState = state;
	m_facing = facing;
	m_playerMap = map;
	m_hp = hp;
	m_teamId = teamid;
	m_mapX = pos.first;
	m_mapY = pos.second;
	m_previousMapX = pos.first;
	m_previousMapY = pos.second;
	m_x = pos.first + 0.5f;
	m_y = pos.second + 0.5f;
	m_spawnpoint = pos;
	m_moveCooldown = kDefaultMoveCooldown;
	m_playerSpeed = 1.0f / m_moveCooldown;
	m_xSpeed = 0.0f;
	m_ySpeed = 0.0f;
	m_isMoving = false;
	m_endOfMove = Clock::now();
	m_lastShotTime = Clock::now();
	m_lastDeathTime = Clock::now();
}