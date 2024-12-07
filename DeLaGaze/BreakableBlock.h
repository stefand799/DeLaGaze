#pragma once
#include "Wall.h"

class BreakableBlock: public Wall{
public:
	BreakableBlock(std::pair<size_t, size_t> pos);
	ObjectType GetType() const override { return ObjectType::BreakableBlock; };
	virtual void Render() override;
	virtual void Print() const override;
	virtual void OnBreak();
	bool CanMoveHere(int i, int j) override;
	virtual crow::json::wvalue toJson() override;
private:
};

