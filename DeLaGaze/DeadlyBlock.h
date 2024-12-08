#pragma once
#include "Block.h"
class DeadlyBlock : public Block
{
public:
	DeadlyBlock(std::pair<size_t, size_t> pos);
	ObjectType GetType() const override { return ObjectType::Pathway; };
	void Render() override;
	void Print() const override;
	virtual crow::json::wvalue toJson() override;
	bool CanMoveHere(int i, int j) override;
private:
};

