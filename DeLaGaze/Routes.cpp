#include "Routes.h"

Routes::Routes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/players")([&]() {
        try {
            std::vector<crow::json::wvalue> players_json;

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
    CROW_ROUTE(app, "/players/add")([&](const crow::request& req) {
        try {
            crow::json::rvalue body = crow::json::load(req.body);
            if (!body) {
                return crow::response(400, "Invalid JSON body");
            }

            Player new_player;
            new_player.SetUsername(body["username"].s());
            new_player.SetScore(body["score"].i());
            new_player.SetPoints(body["points"].i());
            new_player.SetBulletSpeed(body["firerate"].i());
            new_player.SetBulletSpeedUpgrade(body["upgrade_bs"].i());

            if (player_storage.AddPlayer(new_player)) {
                return crow::response(201, "Player added successfully");
            }
            else {
                return crow::response(500, "Error adding player");
            }
        }
        catch (const std::exception& e) {
            return crow::response(500, "Internal server error: " + std::string(e.what()));
        }
        });
}
