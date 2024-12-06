#include "Block.h"

Block::Block(std::pair<int, int> pos) :
	m_pos{pos}
{
}

void Block::Render() {
	//Should default block have a render?
}

std::pair<int, int> Block::GetPos()
{
	return m_pos;
}

//void Block::Print() const {
//	std::cout << "W";
//}

