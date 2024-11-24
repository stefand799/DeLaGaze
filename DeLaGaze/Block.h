#pragma once
#include "Object.h"

class Block : public Object {
public:
	virtual void Render() override =0;
	//OR
	//void render(int x, int y) override;
	virtual void Print() override =0;
private:

};
