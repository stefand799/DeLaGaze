#pragma once
#include "Object.h"

class Block : public Object {
public:
	Block(std::pair<int, int> pos);
	virtual void Render() override =0;
	//OR
	//void render(int x, int y) override;
	std::pair<int, int> GetPos();
	virtual void Print() const override =0;
	virtual crow::json::wvalue toJson() override = 0;
protected:
	std::pair<int, int> m_pos;
private:

};
