#pragma once
#include "Object.h"
#include "Direction.h"
class Bullet : public Object
{
public:
	ObjectType GetType() const override { return ObjectType::Bullet; };
	void Render() override;
	void Print() override;
	bool CanMoveHere(int i, int j) override;
	void Move();
	std::pair<int, int> GetPosition() const { return m_position; };
	Bullet()=delete;
	Bullet(std::pair<float,float> pos, float speed, Direction direction);
private:
	std::pair<float,float> m_position; //TODO: delete the inherited int,int m_position
	float m_speed;
	Direction m_direction;
};