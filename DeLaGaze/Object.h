#pragma once

#include <iostream>
#include <utility>

class Object {
public:
	 Object(const std::pair<int,int>& pos);

	virtual void render() = 0;
	//OR
	//virtual void render(int x, int y) = 0;
	virtual void print() = 0;
	virtual bool canMoveHere(int i,int j) = 0;
private:
	std::pair<int, int> m_pos;
};