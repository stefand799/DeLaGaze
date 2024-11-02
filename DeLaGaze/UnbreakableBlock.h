#pragma once
#include "Block.h"

class UnbreakableBlock : public Block{
public:
	void render() override;
	void print() override;
private:
};

