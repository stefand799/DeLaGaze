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
	Player() = delete;
	Player(Map& m);
	Player(const int& i, const int& j, Map& m);
	Player(Map& m, const std::pair<int,int>& pos, const std::string_view& username, uint8_t points, uint8_t speed, Direction Facing, State PlayerState, uint8_t score = 0, uint8_t hp = 3);
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
	std::string GetUsername() const;
	void SetUsername(const std::string_view& username);
	void SetFacing(const Direction& facing);
	void SetPlayerState(const State& playerState);
	Direction GetFacing() const;
	State GetPlayerState() const;
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void Shoot(std::vector<std::shared_ptr<Bullet>>& bullets);
	void CommitSprite();
	void Respawn();
private:
	Map& m_playerMap;
	uint8_t m_hp : 2;
	uint8_t m_score;
	uint16_t m_points;
	int m_x, m_y;
	uint8_t m_speed;
	Direction m_facing;
	State m_playerState;
	std::vector<Bullet*> m_bullets;
	std::string m_username;
	bool m_bulletSpeedUpgrade;
	float shootCooldown;
	std::pair<int, int> m_spawnpoint;
};

