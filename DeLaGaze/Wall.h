#pragma once
#include "Block.h"
class Wall:public Block{
public:
	Wall(std::pair<size_t, size_t> pos);
	bool CanMoveHere() override;
	virtual crow::json::wvalue toJson() override = 0;
private:
};

