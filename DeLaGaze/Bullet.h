#pragma once
#include "Object.h"
class Bullet : public Object
{
public:
	ObjectType GetType() const override { return ObjectType::Bullet; };
	void Render() override;
	void Print() override;
	bool CanMoveHere(int i, int j) override;
	void Move();
	std::pair<int, int> GetPosition() const { return m_position; };
	Bullet()=default;
	Bullet(uint8_t speed);
private:
	std::pair<int, int>m_position;
	uint8_t m_speed;
};