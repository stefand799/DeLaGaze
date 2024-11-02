#pragma once
#include "Block.h"

class BreakableBlock: public Block{
public:
	virtual void render() override;
	virtual void print() override;
	virtual void onBreak();
private:
};

