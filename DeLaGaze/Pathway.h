#pragma once
#include "Block.h"
class Pathway: public Block{
public:
	void render() override;
	void print() override;
	bool canMoveHere(int i, int j) override;
private:
};

