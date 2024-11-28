#include "Bullet.h"

Bullet::Bullet(float x, float y, float speed, Direction direction) :
	m_x{x},
	m_y{y},
	m_speed{speed},
	m_direction{ direction }
{
	if (m_direction == Direction::North) {
		m_y -= 1;
	}
	if (m_direction == Direction::South) {
		m_y += 1;
	}
	if (m_direction == Direction::West) {
		m_x -= 1;
	}
	if (m_direction == Direction::East) {
		m_x += 1;
	}
	m_prevX = m_x;
	m_prevY = m_y;
}

void Bullet::Render()
{
}

void Bullet::Print() const
{
	//std::cout << "\033[37;42m" << "*" << "\033[0m";	if(m_facing == Direction::North) std::cout << "\033[37;41m" << "^" << "\033[0m";
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
	float newX = m_x;
	float newY = m_y;
	if (m_direction == Direction::North) {
		newY -= m_speed * deltaTime;
	}
	if (m_direction == Direction::South) {
		newY += m_speed * deltaTime;
	}
	if (m_direction == Direction::West) {
		newX -= m_speed * deltaTime;
	}
	if (m_direction == Direction::East) {
		newX += m_speed * deltaTime;
	}
	m_x = newX;
	m_y = newY;
	//TODO: verify if the bullet is still on the map. If it is, then place it there and then call check colisions in update(). If it is not, then call its delete?
}

float Bullet::GetX() const
{
	return m_x;
}

float Bullet::GetY() const
{
	return m_y;
}



inline Object::ObjectType Bullet::GetType() const { return ObjectType::Bullet; }

