#pragma once
#include <string>
#include <vector>
#include <optional>
#include <tuple>
#include "Player.h"
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;

namespace database
{
    inline auto CreateStorage(const std::string& filename)
    {
        return sql::make_storage(
            filename,
            sql::make_table(
                "Players",
                sql::make_column("username", &Player::SetUsername, &Player::GetUsername, sql::primary_key()),
                sql::make_column("score", &Player::SetScore, &Player::GetScore),
                sql::make_column("points", &Player::SetPoints, &Player::GetPoints),
                sql::make_column("firerate", &Player::SetBulletSpeed, &Player::GetBulletSpeed),
                sql::make_column("upgrade_bs", &Player::SetBulletSpeedUpgrade, &Player::GetBulletSpeedUpgrade)
            )
        );
    }

    using Storage = decltype(CreateStorage(""));

    class PlayerStorage
    {
    public:
        // Constructor pentru sincronizarea schemei bazei de date
        PlayerStorage();

        // Metode CRUD pentru tabela Players
        bool AddPlayer(const Player& player);
        std::optional<Player> GetPlayer(const std::string& username);
        bool UpdatePlayer(const Player& player);
        bool DeletePlayer(const std::string& username);
        std::vector<Player> GetAllPlayers();

    private:
        void PopulateStorage();

    private:
        const std::string kDbFile{ "players.sqlite" };
        Storage m_db = CreateStorage(kDbFile);
    };
}
