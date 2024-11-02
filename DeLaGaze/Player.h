#pragma once
#include "Object.h";
#include <cstdint>
class Player: public Object{
private:
	uint8_t m_hp : 2;
	uint8_t m_score;
	int m_points;
	int m_x, m_y;
public:
	Player();
	~Player();
	void render() override;
	void print() override;
	void SetX(int x);
	void SetY(int y);
	int GetX();
	int GetY();
	void setHp(uint8_t hp);
	void SetScore(uint8_t score);
	void SetPoints(int points);
	uint8_t GetHp();
	uint8_t GetScore();
	int GetPoints();
};

