#pragma once
#include "Block.h"
class Wall:public Block{
	virtual void Render() override = 0;
	virtual void Print() const override = 0;
};

