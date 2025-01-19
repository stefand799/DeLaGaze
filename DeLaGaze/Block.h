#pragma once
#include "Object.h"

class Block : public Object {
public:
	Block(std::pair<size_t, size_t> pos);
	std::pair<int, int> GetPos();
	virtual crow::json::wvalue toJson() override = 0;
protected:
	std::pair<size_t, size_t> m_pos;
private:

};
