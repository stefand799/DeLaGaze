#pragma once

#include <iostream>
#include <utility>

class Object {
public:
	enum class ObjectType {
		Player,
		Bullet,
		Pathway,
		UnbreakableBlock,
		BreakableBlock,
		BombTrapBlock
	};
	virtual ObjectType GetType() const = 0;

	//Object(const std::pair<int,int>& pos);

	virtual void Render() = 0;
	virtual void Print() const = 0;
	virtual bool CanMoveHere(int i,int j) = 0; //TODO: Remove parameters, it already knows it's position from m_pos.
private:
	std::pair<int, int> m_pos;
};