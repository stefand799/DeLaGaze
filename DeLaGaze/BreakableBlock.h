#pragma once
#include "Wall.h"

class BreakableBlock: public Wall{
public:
	ObjectType GetType() const override { return ObjectType::BreakableBlock; };
	virtual void Render() override;
	virtual void Print() override;
	virtual void OnBreak();
	bool CanMoveHere(int i, int j) override;
private:
};

