#include "game.h"
#include <crow.h>
#include <cpr/cpr.h>     
#include <sqlite_orm/sqlite_orm.h>

int main() {
	Game game;
	game.start();

	std::cout << "Rulat cu succes";
	return 0;
}