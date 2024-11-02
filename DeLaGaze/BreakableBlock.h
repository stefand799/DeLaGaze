#pragma once
#include "Block.h"

class BreakableBlock: public Block{
public:
	void render() override;
	void print() override;
	void onBreak();
private:
};

