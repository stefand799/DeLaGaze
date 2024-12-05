#include "Bullet.h"

Bullet::Bullet(float x, float y, float speed, Direction direction) :
	m_x{x},
	m_y{y},
	m_speed{speed},
	m_direction{ direction },
	m_xSpeed {0},
	m_ySpeed {0}
{
	if (m_direction == Direction::North) {
		//m_y -= 1;
		m_ySpeed = -m_speed;
	}
	if (m_direction == Direction::South) {
		m_y += 1;
		m_ySpeed = +m_speed;
	}
	if (m_direction == Direction::West) {
		//m_x -= 1;
		m_xSpeed = -m_speed;
	}
	if (m_direction == Direction::East) {
		m_x += 1;
		m_xSpeed = +m_speed;
	}
}

void Bullet::Render()
{
}

void Bullet::Print() const
{
	if (m_direction == Direction::North) std::cout << "\033[37;42m" << "^" << "\033[0m";
	if (m_direction == Direction::South) std::cout << "\033[37;42m" << "v" << "\033[0m";
	if (m_direction == Direction::West) std::cout << "\033[37;42m" << "<" << "\033[0m";
	if (m_direction == Direction::East) std::cout << "\033[37;42m" << ">" << "\033[0m";
}

bool Bullet::CanMoveHere(int i,int j) {
	return true;
}

void Bullet::Move(float deltaTime)
{
	/*TODO: Implement alongside QT*/
	if (deltaTime < 0.0f) return;
	m_x = m_x + m_xSpeed * deltaTime;
	m_y = m_y + m_ySpeed * deltaTime;
}

float Bullet::GetX() const
{
	return m_x;
}

float Bullet::GetY() const
{
	return m_y;
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



inline Object::ObjectType Bullet::GetType() const { return ObjectType::Bullet; }

std::tuple<Object*, Object*, float> GetBulletToBulletColision(Bullet* first, Bullet* second)
{
	float x12 = first->m_x - second->m_x;
	float y12 = first->m_y - second->m_y;
	float vx12 = first->m_xSpeed - second->m_xSpeed;
	float vy12 = first->m_ySpeed - second->m_ySpeed;
	
	float a = vx12 * vx12 + vy12 * vy12;
	float b = 2 * (x12 * vx12 + y12 * vy12);
	float c = x12 * x12 + y12 * y12 - (first->kRadius + second->kRadius)*(first->kRadius + second->kRadius);

	float delta = (b * b) - (4 * a * c);

	if (delta < 0) return { first,second,0 };

	float sqrtDelta = sqrt(delta);
	float t1 = (-b - sqrtDelta) / (2 * a);
	float t2 = (-b + sqrtDelta) / (2 * a);

	if (t1 < 0 && t2 < 0) return { first,second,0 };

	if (t1 < 0) return { first, second, t2 };
	if (t2 < 0) return { first, second, t1 };
	return { first, second, std::min(t1,t2) };
}
