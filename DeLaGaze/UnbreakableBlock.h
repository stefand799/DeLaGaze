#pragma once
#include "Wall.h"

class UnbreakableBlock : public Wall{
public:
	UnbreakableBlock(std::pair<size_t, size_t> pos);
	ObjectType GetType() const override { return ObjectType::UnbreakableBlock; };
	 void Print() const override;
	 virtual crow::json::wvalue toJson() override;
private:
};

