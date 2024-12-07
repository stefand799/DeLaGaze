#pragma once
#include "Wall.h"

class UnbreakableBlock : public Wall{
public:
	UnbreakableBlock(std::pair<size_t, size_t> pos);
	ObjectType GetType() const override { return ObjectType::UnbreakableBlock; };
	 void Render() override;
	 void Print() const override;
	 bool CanMoveHere(int i, int j) override;
	 virtual crow::json::wvalue toJson() override;
private:
};

