#pragma once
#include "Wall.h"

class UnbreakableBlock : public Wall{
public:
	ObjectType getType() const override { return ObjectType::UnbreakableBlock; };
	 void render() override;
	 void print() override;
	 bool canMoveHere(int i, int j) override;
private:
};

