#pragma once
#include "Object.h"

class Block : public Object {
public:
	void render() override;
	//OR
	//void render(int x, int y) override;
	void print() override;
private:

};
