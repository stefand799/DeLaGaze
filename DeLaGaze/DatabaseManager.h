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
                sql::make_column("id",&Player::SetId,&Player::GetId,sql::primary_key()),
                sql::make_column("username", &Player::SetUsername, &Player::GetUsername),
                sql::make_column("score", &Player::SetScore, &Player::GetScore),
                sql::make_column("points", &Player::SetPoints, &Player::GetPoints),
                sql::make_column("firerate", &Player::SetFireRate, &Player::GetFireRate),
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
        bool AddPlayer(const std::shared_ptr<Player>& player);
        std::shared_ptr<Player> GetPlayer(const std::string& username);
        bool UpdatePlayer(const std::shared_ptr<Player>& player);
        bool DeletePlayer(const std::string& username);
        std::vector<std::shared_ptr<Player>> GetAllPlayers();
        std::shared_ptr<Player> GetPlayerByName(const std::vector<std::shared_ptr<Player>>& playersVector,
            const std::string& name);
    private:
        void PopulateStorage();

    private:
        const std::string kDbFile{ "players.sqlite" };
        Storage m_db = CreateStorage(kDbFile);
    };
}
