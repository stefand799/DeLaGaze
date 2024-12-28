#pragma once
#include "Object.h"
#include "Direction.h"
#include "Player.h"
class Bullet : public Object
{
public:
	Bullet()=delete;
	Bullet(std::shared_ptr<Player> owner, float x, float y, float speed, Direction direction);
	~Bullet() = default;
	ObjectType GetType() const override;
	void Print() const override;
	bool CanMoveHere() override;
	void Move(float deltaTime);
	std::shared_ptr<Player> GetOwner() const;
	float GetX() const;
	float GetY() const;
	float GetXSpeed() const;
	float GetYSpeed() const;
	float GetRadius() const;
	friend std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float> GetBulletToBulletColision(std::shared_ptr<Bullet> first, std::shared_ptr<Bullet> second);
	virtual crow::json::wvalue toJson() override;
	std::string DirectionToString(Direction direction);
	//std::pair<int, int> GetPosition() const { return {(int)m_x,(int)m_y}; };
private:
	//Constants
	const float kRadius = 0.15f;
	std::shared_ptr<Player> m_owner;
	float m_x;
	float m_y;
	float m_speed;
	float m_xSpeed;
	float m_ySpeed;
	Direction m_direction;
};