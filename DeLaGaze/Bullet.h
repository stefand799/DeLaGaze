#pragma once
#include "Object.h"
#include "Direction.h"
class Bullet : public Object
{
public:
	Bullet()=delete;
	Bullet(float x, float y, float speed, Direction direction);
	ObjectType GetType() const override;
	void Render() override;
	void Print() const override;
	bool CanMoveHere(int i, int j) override;
	void Move(float deltaTime);
	//std::pair<int, int> GetPosition() const { return {(int)m_x,(int)m_y}; };
public:
	float GetX() const;
	float GetY() const;
private:
	//TODO: delete the inherited int,int m_position
	float m_x;
	float m_y;
	float m_prevX;
	float m_prevY;
	float m_speed;
	Direction m_direction;
};