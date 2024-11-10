#pragma once
#include "Wall.h"

class BreakableBlock: public Wall{
public:
	virtual void render() override;
	virtual void print() override;
	virtual void onBreak();
	bool canMoveHere(int i, int j) override;
private:
};

