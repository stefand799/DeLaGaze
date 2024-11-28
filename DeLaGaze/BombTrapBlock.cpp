#include "BombTrapBlock.h"

void BombTrapBlock::Render(){
}

void BombTrapBlock::Print() const{
	std::cout << "\033[30;43m" << "*" << "\033[0m"; //X
}

void BombTrapBlock::OnBreak(){
}
