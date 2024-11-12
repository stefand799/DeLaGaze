#pragma once
#include "Wall.h"

class BreakableBlock: public Wall{
public:
	ObjectType getType() const override { return ObjectType::BreakableBlock; };
	virtual void render() override;
	virtual void print() override;
	virtual void onBreak();
	bool canMoveHere(int i, int j) override;
private:
};

