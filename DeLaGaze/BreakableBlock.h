#pragma once
#include "Wall.h"

class BreakableBlock: public Wall{
public:
	virtual void render() override;
	virtual void print() override;
	virtual void onBreak();
private:
};

