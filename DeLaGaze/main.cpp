#include "Game.h"
#include "DatabaseManager.h"
#include "Routes.h"

int main() {
    Game game;
    game.Start();
    database::PlayerStorage playerStorage;
    Routes routes(playerStorage, game);
    std::vector<std::shared_ptr<Player>> p = playerStorage.GetAllPlayers();
	for (const auto& player : p) {
		std::cout << player->GetUsername() << " " << player->GetBulletSpeedUpgrade()<<" " << static_cast<unsigned int>(player->GetFireRate()) << std::endl;
	}
    routes.Run(playerStorage);
    return 0;
}