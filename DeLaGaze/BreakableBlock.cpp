#include "BreakableBlock.h"

void BreakableBlock::render(){
}

void BreakableBlock::print() {
	std::cout << "B";
}

void BreakableBlock::onBreak(){
}

bool BreakableBlock::canMoveHere(int i, int j){
	return false;
}
