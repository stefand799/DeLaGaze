#include "Game.h"
#include "DatabaseManager.h"
#include "Routes.h"

int main() {
    Game game;
    game.Start();
    database::PlayerStorage playerStorage;
    Routes routes(playerStorage, game);
    routes.Run(playerStorage);
    return 0;
}