#include "BreakableBlock.h"

void BreakableBlock::Render(){
}

void BreakableBlock::Print() {
	std::cout << "B";
}

void BreakableBlock::OnBreak(){
}

bool BreakableBlock::CanMoveHere(int i, int j){
	return false;
}
