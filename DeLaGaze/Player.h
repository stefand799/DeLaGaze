#pragma once
#include "Object.h";
#include <cstdint>
class Player: public Object{
private:
	uint8_t hp : 2;
	uint8_t score;
	int points;
	int x, y;
};

