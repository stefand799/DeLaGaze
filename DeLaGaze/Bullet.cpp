#include "Bullet.h"

void Bullet::render()
{
}

void Bullet::print()
{
	std::cout << "Bullet\n";
}

bool Bullet::canMoveHere(int i,int j){
	return true;
}

void Bullet::move(){
	/*TODO: Implement alongside QT*/
}

Bullet::Bullet(uint8_t speed) : m_speed{ speed } {};