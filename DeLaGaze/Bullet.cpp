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

Bullet::Bullet(uint8_t speed) : m_speed{ speed } {};