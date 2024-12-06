#pragma once
#include "Block.h"
class Wall:public Block{
public:
	Wall(std::pair<int, int> pos);
	virtual void Render() override = 0;
	virtual void Print() const override = 0;
	virtual crow::json::wvalue toJson() override = 0;
private:
};

