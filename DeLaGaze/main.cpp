#include "game.h"
#include "DatabaseManager.h"
#include <crow.h>
#include <cpr/cpr.h>     
#include <sqlite_orm/sqlite_orm.h>
#include "Routes.h"
#include "DatabaseManager.h"
// We comment libraries we don't use to not waste time at compilation when developing


int main() {
	Game game;
	game.Start();
	database::PlayerStorage playerStorage;
	Routes r(playerStorage);
	r.Run(playerStorage);
	std::cout << "Rulat cu succes";
	return 0;
}