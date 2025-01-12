#include "DatabaseManager.h"

namespace database
{
    PlayerStorage::PlayerStorage()
    {
        m_db.sync_schema();
        PopulateStorage();
    }
    void PlayerStorage::PopulateStorage()
    {
        try {

            if (m_db.count<Player>() > 0) {
                m_db.insert(Player(1, "ppp1", 1000, 1000, 1, false));
                m_db.insert(Player(2, "ppp2", 1000, 1000, 1, false));
            
                std::cout << "Database populated with default players." << std::endl;
            }
            else {
                std::cout << "Database already contains players. Skipping population." << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error populating databse: " << e.what() << std::endl;
        }
    }

    bool PlayerStorage::AddPlayer(const std::shared_ptr<Player>& player)
    {
        try {
            m_db.insert(Player{
                player->GetId(),
                player->GetUsername(),
                player->GetScore(),
                player->GetPoints(),
                player->GetFireRate(),
                player->GetBulletSpeedUpgrade()
                });
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error adding player: " << e.what() << std::endl;
            return false;
        }
    }

    std::shared_ptr<Player> PlayerStorage::GetPlayer(const std::string& username)
    {
        try {
            auto player = m_db.get_optional<Player>(username);
            return  std::make_shared<Player>(*player);
        }
        catch (const std::exception& e) {
            std::cerr << "Error getting player: " << e.what() << std::endl;
            return nullptr;
        }
    }

    bool PlayerStorage::UpdatePlayer(const std::shared_ptr<Player>& player)
    {
        try {
            m_db.update(*player);
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error updating player: " << e.what() << std::endl;
            return false;
        }
    }

    bool PlayerStorage::DeletePlayer(const std::string& username)
    {
        try {
            m_db.remove<Player>(username);
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error deleting player: " << e.what() << std::endl;
            return false;
        }
    }

    std::vector<std::shared_ptr<Player>> PlayerStorage::GetAllPlayers()
    {
        try {
            auto players = m_db.get_all<Player>();
            std::vector<std::shared_ptr<Player>> playerPtrs;
            playerPtrs.reserve(players.size());

            for (const auto& player : players) {
                playerPtrs.emplace_back(std::make_shared<Player>(player));
            }

            return playerPtrs;
        }
        catch (const std::exception& e) {
            std::cerr << "Error getting all players: " << e.what() << std::endl;
            return {};
        }
    }

    std::shared_ptr<Player> PlayerStorage::GetPlayerByName(const std::vector<std::shared_ptr<Player>>& playersVector, const std::string& name) {
        for (const auto& player : playersVector) {
                if (player && player->GetUsername() == name) {
                    return player;
                }
        }
        return nullptr;
    }
}
