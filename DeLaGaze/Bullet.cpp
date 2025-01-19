#include "Bullet.h"

Bullet::Bullet(std::shared_ptr<Player> owner, float x, float y, float speed, Direction direction) :
	m_owner{ owner },
	m_x{ x },
	m_y{ y },
	m_speed{ speed },
	m_direction{ direction },
	m_xSpeed{ 0 },
	m_ySpeed{ 0 }
{
	if (m_direction == Direction::North) {
		m_y -= 0.5f;
		m_ySpeed = -m_speed;
	}
	if (m_direction == Direction::South) {
		m_y += 0.5f;
		m_ySpeed = +m_speed;
	}
	if (m_direction == Direction::West) {
		m_x -= 0.5f;
		m_xSpeed = -m_speed;
	}
	if (m_direction == Direction::East) {
		m_x += 0.5f;
		m_xSpeed = +m_speed;
	}
}

float Bullet::GetX() const
{
	return m_x;
}
float Bullet::GetY() const
{
	return m_y;
}
float Bullet::GetXSpeed() const
{
	return m_xSpeed;
}
float Bullet::GetYSpeed() const
{
	return m_ySpeed;
}
float Bullet::GetRadius() const
{
	return kRadius;
}
inline Object::ObjectType Bullet::GetType() const 
{ 
	return ObjectType::Bullet; }
std::shared_ptr<Player> Bullet::GetOwner() const
{
	return m_owner;
}

void Bullet::Move(float deltaTime)
{
	if (deltaTime < 0.0f) return;
	m_x = m_x + m_xSpeed * deltaTime;
	m_y = m_y + m_ySpeed * deltaTime;
}
bool Bullet::CanMoveHere() {
	return true;
}
std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object>, float> GetBulletToBulletColision(std::shared_ptr<Bullet> first, std::shared_ptr<Bullet> second)
{
	float x12 = first->m_x - second->m_x;
	float y12 = first->m_y - second->m_y;
	float vx12 = first->m_xSpeed - second->m_xSpeed;
	float vy12 = first->m_ySpeed - second->m_ySpeed;

	float a = vx12 * vx12 + vy12 * vy12;
	float b = 2 * (x12 * vx12 + y12 * vy12);
	float c = x12 * x12 + y12 * y12 - (first->kRadius + second->kRadius) * (first->kRadius + second->kRadius);
	if (c <= 0.0f) return { first, second, 0.0f };
	float delta = (b * b) - (4 * a * c);

	if (delta < 0.0f || a == 0.0f) return { first,second,NAN };

	float sqrtDelta = sqrt(delta);
	float t1 = (-b - sqrtDelta) / (2 * a);
	float t2 = (-b + sqrtDelta) / (2 * a);

	if (t1 < 0.0f && t2 < 0.0f) return { first,second, NAN };

	if (t1 < 0.0f) return { first, second, t2 };
	if (t2 < 0.0f) return { first, second, t1 };
	return { first, second, std::min(t1,t2) };
}
std::string Bullet::DirectionToString(Direction direction)
{
	switch (direction) {
	case Direction::North: return "North";
	case Direction::East: return "East";
	case Direction::South: return "South";
	case Direction::West: return "West";
	default: return "Unknown";
	}
}
crow::json::wvalue Bullet::toJson()
{
	crow::json::wvalue jsonObj;
	jsonObj["type"] = "Bullet";
	jsonObj["x"] = m_x;
	jsonObj["y"] = m_y;
	jsonObj["direction"] = DirectionToString(m_direction);
	return jsonObj;
}





