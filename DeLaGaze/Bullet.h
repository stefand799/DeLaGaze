#pragma once
#include "Object.h"
class Bullet : public Object
{
public:
	void render() override;
	void print() override;
	bool canMoveHere(int i, int j) override;
};