#pragma once
#include "Object.h"
#include "Direction.h"
#include "Player.h"
class Bullet : public Object
{
public:
	Bullet()=delete;
	Bullet(Player* owner, float x, float y, float speed, Direction direction);
	~Bullet() = default;
	ObjectType GetType() const override;
	void Render() override;
	void Print() const override;
	bool CanMoveHere(int i, int j) override;
	void Move(float deltaTime);
	Player* GetOwner() const;
	float GetX() const;
	float GetY() const;
	float GetXSpeed() const;
	float GetYSpeed() const;
	float GetRadius() const;
	friend std::tuple<Object*, Object*, float> GetBulletToBulletColision(Bullet* first, Bullet* second);
	virtual crow::json::wvalue toJson() override;
	std::string DirectionToString(Direction direction);
	//std::pair<int, int> GetPosition() const { return {(int)m_x,(int)m_y}; };
private:
	//Constants
	const float kRadius = 0.15f;
	Player* m_owner;
	float m_x;
	float m_y;
	float m_speed;
	float m_xSpeed;
	float m_ySpeed;
	Direction m_direction;
};