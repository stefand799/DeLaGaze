#include "DatabaseManager.h"

namespace database
{
    PlayerStorage::PlayerStorage()
    {
        m_db.sync_schema();
    }

    bool PlayerStorage::AddPlayer(const Player& player)
    {
        try {
            m_db.insert(Player{ player.GetId(),
                player.GetUsername(),
                player.GetScore(),
                player.GetPoints(),
                player.GetBulletSpeed()
                ,player.GetBulletSpeedUpgrade() });
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error adding player: " << e.what() << std::endl;
            return false;
        }
    }

    std::optional<Player> PlayerStorage::GetPlayer(const std::string& username)
    {
        try {
            auto player = m_db.get_optional<Player>(username);
            return player;
        }
        catch (const std::exception& e) {
            std::cerr << "Error getting player: " << e.what() << std::endl;
            return std::nullopt;
        }
    }

    bool PlayerStorage::UpdatePlayer(const Player& player)
    {
        try {
            m_db.update(player);
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

    std::vector<Player> PlayerStorage::GetAllPlayers()
    {
        try {
            return m_db.get_all<Player>();
        }
        catch (const std::exception& e) {
            std::cerr << "Error getting all players: " << e.what() << std::endl;
            return {};
        }
    }
}
