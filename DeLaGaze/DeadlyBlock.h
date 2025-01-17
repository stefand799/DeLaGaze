#pragma once
#include "UnbreakableBlock.h"

class DeadlyBlock : public UnbreakableBlock
{
public:
	DeadlyBlock(std::pair<size_t, size_t> pos);
	void Print() const override;
	ObjectType GetType() const override { return ObjectType::DeadlyBlock; };
	virtual crow::json::wvalue toJson() override;
	bool CanMoveHere() override;
private:
};

