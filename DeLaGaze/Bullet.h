#pragma once
#include "Object.h"
class Bullet : public Object
{
public:
	ObjectType getType() const override { return ObjectType::Bullet; };
	void render() override;
	void print() override;
	bool canMoveHere(int i, int j) override;
};