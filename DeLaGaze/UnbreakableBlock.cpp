#include "UnbreakableBlock.h"

void UnbreakableBlock::Render(){
}

void UnbreakableBlock::Print(){
	std::cout << "U";
}

bool UnbreakableBlock::CanMoveHere(int i, int j){
	return false;
}
