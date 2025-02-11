#pragma once
#include "Wall.h"

class BreakableBlock: public Wall{
public:
	BreakableBlock(std::pair<size_t, size_t> pos);
	ObjectType GetType() const override { return ObjectType::BreakableBlock; };
	virtual void OnBreak();
	virtual crow::json::wvalue toJson() override;
private:
};

