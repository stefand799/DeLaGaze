#pragma once
#include "Object.h"
#include "Direction.h"
#include "Player.h"
class Bullet : public Object
{
public:
	// Constructors
	Bullet()=delete;
	Bullet(std::shared_ptr<Player> owner, float x, float y, float speed, Direction direction);
	// Destructors
	~Bullet() = default;
	// Getters
	float GetX() const;
	float GetY() const;
	float GetXSpeed() const;
	float GetYSpeed() const;
	float GetRadius() const;
	ObjectType GetType() const override;
	std::shared_ptr<Player> GetOwner() const;
	// Functionalities
	void Move(float deltaTime);
	bool CanMoveHere() override;
	friend std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float> GetBulletToBulletColision(std::shared_ptr<Bullet> first, std::shared_ptr<Bullet> second);
	virtual crow::json::wvalue toJson() override;
	std::string DirectionToString(Direction direction);
	void Print() const override;
private:
	//Constants
	const float kRadius = 0.15f;
	// Attributs
	std::shared_ptr<Player> m_owner;
	float m_x;
	float m_y;
	float m_speed;
	float m_xSpeed;
	float m_ySpeed;
	Direction m_direction;
};