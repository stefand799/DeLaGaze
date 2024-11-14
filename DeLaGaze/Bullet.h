#pragma once
#include "Object.h"
class Bullet : public Object
{
public:
	ObjectType getType() const override { return ObjectType::Bullet; };
	void render() override;
	void print() override;
	bool canMoveHere(int i, int j) override;
	void move();
	std::pair<int, int> getPosition() const { return m_position; };
private:
	std::pair<int, int>m_position;
};