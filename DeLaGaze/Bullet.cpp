#include "Bullet.h"

void Bullet::Render()
{
}

void Bullet::Print()
{
	std::cout << "Bullet\n";
}

bool Bullet::CanMoveHere(int i,int j){
	return true;
}

void Bullet::Move(){
	/*TODO: Implement alongside QT*/
}

Bullet::Bullet(std::pair<float, float> pos, float speed, Direction direction) :
	m_position{pos},
	m_speed{speed},
	m_direction{ direction }
{}

