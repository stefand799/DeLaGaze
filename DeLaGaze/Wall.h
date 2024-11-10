#pragma once
#include "Block.h"
class Wall:public Block{
	virtual void render() override = 0;
	virtual void print() override = 0;
};

