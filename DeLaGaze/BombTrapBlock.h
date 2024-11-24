#pragma once
#include "BreakableBlock.h"
class BombTrapBlock: public BreakableBlock{
public:
	ObjectType GetType() const override { return ObjectType::BombTrapBlock; };
	void Render() override;
	void Print() override;
	void OnBreak() override;
private:
};

