#pragma once
#include "BreakableBlock.h"
class BombTrapBlock: public BreakableBlock{
public:
	ObjectType getType() const override { return ObjectType::BombTrapBlock; };
	void render() override;
	void print() override;
	void onBreak() override;
private:
};

