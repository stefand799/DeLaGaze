#include "Block.h"

Block::Block(std::pair<size_t, size_t> pos) :
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

