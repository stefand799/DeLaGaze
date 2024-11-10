#include "UnbreakableBlock.h"

void UnbreakableBlock::render(){
}

void UnbreakableBlock::print(){
	std::cout << "U";
}

bool UnbreakableBlock::canMoveHere(int i, int j){
	return false;
}
