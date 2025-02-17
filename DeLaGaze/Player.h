#pragma once
#include <vector>
#include <utility>
#include <cstdint>
#include <chrono>
#include "Object.h"
#include "Direction.h"
#include "State.h"
#include "Map.h"
#include "Pathway.h"


class Bullet;

class Player: public Object {
public:
	ObjectType GetType() const override { return ObjectType::Player; };
	
	Player() = default;
	Player(Map* m, const std::pair<int,int>& pos, const int& id, const std::string& username, uint8_t points, uint8_t fireRate, bool bulletSpeedUpgrade, Direction facing, State playerState, uint8_t score = 0, uint8_t teamid = 0, uint8_t hp = 3);
	Player(const int& id, const std::string& username, uint8_t score, uint16_t points, uint8_t fireRate, bool bulletSpeedUpgrade);
	~Player() = default;

	Player& operator=(const Player& other);
	const int GetId() const;
	const std::string GetUsername() const;
	uint8_t GetScore() const;
	uint16_t GetPoints() const;
	uint8_t GetFireRate() const;
	const bool GetBulletSpeedUpgrade() const;
	uint8_t GetHp() const;
	float GetX() const;
	float GetY() const;
	bool GetMovingState() const;
	State GetPlayerState() const;	
	Direction GetFacing() const;
	Map* GetMap() const;
	uint8_t GetTeamId() const;
	std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float> GetBulletToPlayerColision(std::shared_ptr<Bullet> bullet);

	void SetId(int id);
	void SetUsername(const std::string& username);
	void SetScore(uint8_t score);
	void SetPoints(uint16_t points);
	void SetFireRate(uint8_t fireRate);
	void SetBulletSpeedUpgrade(bool bulletSpeedUpgrade);
	void SetHp(uint8_t hp);
	void SetFacing(const Direction& facing);
	void SetPlayerState(const State& playerState);
	void SetX(const int& x);
	void SetY(const int& y);
	void SetPlayerInGame(Map* map, const std::pair<int, int>& pos, Direction facing, State state, uint8_t teamid, uint8_t hp);

	bool CanMoveHere() override;
	void Move(float deltaTime);
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void FaceNorth();
	void FaceSouth();
	void FaceWest();
	void FaceEast();
	
	void Shoot(std::vector<std::shared_ptr<Bullet>>& bullets);
	void Kill();
	void OnDeath();
	void Respawn();
	virtual crow::json::wvalue toJson() override;
	std::string DirectionToString(Direction direction);
	std::string StateToString(State state);

private:
	using Clock = std::chrono::high_resolution_clock;
	using fSecDur = std::chrono::duration<float>;
	const float kFireRates[4] = {
		1.0f,
		0.5f,
		0.25f,
		0.125f
	};
	const float kBulletSpeeds[2] = {
		2.0f,
		4.0f
	};
	const float kDefaultMoveCooldown = 0.25f;
	const float kPlayerHitBoxRadius = 0.3f;
	const float kSpawnProtection = 3.0f;

	int m_id;
	std::string m_username;
	Map* m_playerMap;
	uint8_t m_hp : 2;
	uint8_t m_score;
	float m_bulletSpeed;
	uint16_t m_points;
	int m_mapX, m_mapY;
	int m_previousMapX, m_previousMapY;
	float m_x, m_y;
	float m_xSpeed, m_ySpeed;
	bool m_bulletSpeedUpgrade;
	uint8_t m_fireRate;
	Direction m_facing;
	State m_playerState;
	float m_shootCooldown;
	Clock::time_point m_lastShotTime;
	float m_moveCooldown;
	float m_playerSpeed;
	bool m_isMoving;
	Clock::time_point m_endOfMove;
	std::pair<int, int> m_spawnpoint;
	Clock::time_point m_lastDeathTime;

	uint8_t m_teamId;
};

