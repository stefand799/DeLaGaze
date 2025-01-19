#pragma once
#include "Block.h"
class Pathway: public Block{
public:
	Pathway(std::pair<size_t, size_t> pos);
	ObjectType GetType() const override { return ObjectType::Pathway; };
	virtual crow::json::wvalue toJson() override;
	bool CanMoveHere() override;
private:
};

