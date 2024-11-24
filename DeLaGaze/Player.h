#pragma once
#include "Object.h"
#include <cstdint>
#include "Direction.h"
#include "State.h"
#include "Bullet.h"
#include <vector>
#include <utility>
#include <cstdint>
#include "Map.h"
#include "Pathway.h"
class Player: public Object{
public:
	ObjectType GetType() const override { return ObjectType::Player; };
	Player() = default;
	Player(Map& m);
	Player(const int& i, const int& j, Map& m);
	Player(Map& m, const std::pair<int,int>& pos, const std::string& username, uint8_t points, bool bulletSpeedUpgrade, Direction facing, State playerState, uint8_t score = 0, uint8_t hp = 3);
	Player(const int& id,const std::string& username, uint8_t score, uint16_t points, uint8_t bulletSpeed, bool bulletSpeedUpgrade)
		: m_id(id),
		m_username(username),
		m_score(score),
		m_points(points),
		m_bulletSpeed(bulletSpeed),
		m_bulletSpeedUpgrade(bulletSpeedUpgrade) {}
	bool CanMoveHere(int i,int j) override;
	void Render() override;
	void Print() override;
	void SetX(const int& x);
	void SetY(const int& y);	
	int GetX() const;
	int GetY() const;
	void SetHp(); //no parameter since we will use GetHp()-1
	void SetScore(uint8_t score);
	void SetPoints(uint16_t points);
	uint8_t GetHp() const;
	uint8_t GetScore() const;
	uint16_t GetPoints() const;
	~Player() = default;
	const std::string GetUsername() const;
	void SetUsername(const std::string& username);
	void SetFacing(const Direction& facing);
	void SetPlayerState(const State& playerState);
	Direction GetFacing() const;
	State GetPlayerState() const;
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void FaceNorth();
	void FaceSouth();
	void FaceWest();
	void FaceEast();
	Direction GetFacing();
	void Shoot(std::vector<std::shared_ptr<Bullet>>& bullets);
	void CommitSprite();
	void Respawn();
private:
	const float kShootingCooldowns[4] = {
		1.0,
		0.5,
		0.25,
		0.125
	};
	const float kBulletSpeeds[2] = {
		0.25,
		0.5
	};
public:
	const bool GetBulletSpeedUpgrade() const;
	void SetBulletSpeedUpgrade(bool bulletSpeedUpgrade);
	const uint8_t GetBulletSpeed() const;
	void SetBulletSpeed(uint8_t bulletSpeed);
	const int GetId() const;
	void SetId(int id);

private:
	int m_id;
	std::string m_username;
	Map m_playerMap;
	uint8_t m_hp : 2;
	uint8_t m_score;
	uint8_t m_bulletSpeed;
	uint16_t m_points;
	int m_x, m_y;
	bool m_bulletSpeedUpgrade;
	Direction m_facing;
	State m_playerState;
	std::vector<Bullet*> m_bullets;
	float shootCooldown;
	std::pair<int, int> m_spawnpoint;
};

