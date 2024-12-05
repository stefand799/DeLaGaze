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
	float GetX() const;
	float GetY() const;
	friend std::tuple<Object*, Object*, float> GetBulletToBulletColision(Bullet* first, Bullet* second);
	virtual crow::json::wvalue toJson() override;
	//std::pair<int, int> GetPosition() const { return {(int)m_x,(int)m_y}; };
private:
	//Constants
	const float kRadius = 0.15f;
	//TODO: delete the inherited int,int m_position
	float m_x;
	float m_y;
	float m_speed;
	float m_xSpeed;
	float m_ySpeed;
	Direction m_direction;
};