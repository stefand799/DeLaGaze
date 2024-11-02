#pragma once
#include "Object.h"

class Block : public Object {
public:
	virtual void render() override =0;
	//OR
	//void render(int x, int y) override;
	virtual void print() override =0;
private:

};
