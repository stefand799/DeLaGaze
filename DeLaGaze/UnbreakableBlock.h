#pragma once
#include "Wall.h"

class UnbreakableBlock : public Wall{
public:
	 void render() override;
	 void print() override;
	 bool canMoveHere(int i, int j) override;
private:
};

