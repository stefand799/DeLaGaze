#include "Game.h"
#include "DatabaseManager.h"
#include "Routes.h"

int main() {
    Game game;
    //game.Start();
    database::PlayerStorage playerStorage;
    Routes routes(playerStorage, game);
    std::vector<std::shared_ptr<Player>> p = playerStorage.GetAllPlayers();
    int cnt = 0;
    for (const auto& player : p) {
        if (player->GetUsername() == "ppp1" || player->GetUsername() == "ppp2") {
            cnt++;
            std::cout << player->GetUsername() << "points: " << static_cast<unsigned int>(player->GetPoints()) << " score:" << static_cast<unsigned int>(player->GetScore()) << std::endl;
			std::cout << player->GetUsername() << "firerate: " << static_cast<unsigned int>(player->GetFireRate()) << " upgrade_bs:" << player->GetBulletSpeedUpgrade() << std::endl;
        }
        if (cnt > 1) break;
    }
    routes.Run(playerStorage);
    return 0;
}