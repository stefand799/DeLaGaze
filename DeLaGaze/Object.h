#pragma once

#include <iostream>

class Object {
public:
	virtual void render() = 0;
	//OR
	//virtual void render(int x, int y) = 0;
	virtual void print() = 0;
	virtual bool canMoveHere(int i,int j) = 0;
private:
};