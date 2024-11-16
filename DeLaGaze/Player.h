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
	ObjectType getType() const override { return ObjectType::Player; };
	Player() = default;
	Player(const std::pair<int,int>& pos, const std::string_view& username, uint8_t points, uint8_t speed, Direction Facing, State PlayerState, uint8_t score = 0, uint8_t hp = 3);
	bool canMoveHere(int i,int j) override;
	void render() override;
	void print() override;
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
	void moveUp(Map& map, const int& i, const int& j);
	void moveDown(Map& map, const int& i, const int& j);
	void moveLeft(Map& map, const int& i, const int& j);
	void moveRight(Map& map, const int& i, const int& j);
	void shoot(std::vector<std::shared_ptr<Bullet>>& bullets);
private:
	uint8_t m_hp : 2;
	uint8_t m_score;
	uint16_t m_points;
	int m_x, m_y;
	uint8_t m_speed;
	Direction m_facing;
	State m_playerState;
	std::vector<Bullet*> m_bullets;
	std::string m_username;
};

