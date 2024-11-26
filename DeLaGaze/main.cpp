#include "game.h"
#include "DatabaseManager.h"
#include <crow.h>
#include <cpr/cpr.h>     
#include <sqlite_orm/sqlite_orm.h>
// We comment libraries we don't use to not waste time at compilation when developing


int main() {
	Game game;
	game.Start();

	std::cout << "Rulat cu succes";
	return 0;
}