#pragma once
#include "BreakableBlock.h"
class BombTrapBlock: public BreakableBlock{
public:
	void render() override;
	void print() override;
	void onBreak() override;
private:
};

