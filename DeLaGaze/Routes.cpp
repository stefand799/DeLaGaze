#include "Routes.h"

Routes::Routes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/players")([&]() {
        try {
            std::vector<crow::json::wvalue> players_json;

            // Iterate through all players and add to JSON response
            for (const auto& player : player_storage.GetAllPlayers()) {
                players_json.push_back(crow::json::wvalue{
                    {"username", player.GetUsername()},
                    {"score", player.GetScore()},
                    {"points", player.GetPoints()},
                    {"firerate", player.GetBulletSpeed()},
                    {"upgrade_bs", player.GetBulletSpeedUpgrade()}
                    });
            }

            return crow::json::wvalue{ players_json };
        }
        catch (const std::exception& e) {
            return crow::json::wvalue{
                {"code", 500},
                {"message", e.what()}
            };
        }
    });
}
