#pragma once
#include "Block.h"
class Pathway: public Block{
public:
	ObjectType getType() const override { return ObjectType::Pathway; };
	void render() override;
	void print() override;
	bool canMoveHere(int i, int j) override;
private:
};

