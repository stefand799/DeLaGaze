#pragma once
#include "Block.h"

class UnbreakableBlock : public Block{
public:
	virtual void render() override;
	virtual void print() override;
private:
};

