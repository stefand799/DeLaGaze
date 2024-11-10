#pragma once
#include "Wall.h"

class UnbreakableBlock : public Wall{
public:
	virtual void render() override;
	virtual void print() override;
private:
};

