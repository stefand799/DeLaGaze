#pragma once
#include "Block.h"
class Pathway: public Block{
public:
	Pathway(std::pair<size_t, size_t> pos);
	ObjectType GetType() const override { return ObjectType::Pathway; };
	void Render() override;
	void Print() const override;
	virtual crow::json::wvalue toJson() override;
	bool CanMoveHere(int i, int j) override;
private:
};

