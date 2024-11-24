#include "game.h"
//#include <crow.h>
//#include <cpr/cpr.h>     
//#include <sqlite_orm/sqlite_orm.h>
// We comment libraries we don't use to not waste time at compilation when developing


int main() {
	Game game;
	game.Start();
    /*Map m_map;
    m_map.generate();
    delete m_map[0][0];
    Player* player = new Player;
    m_map[0][0] = player;
    player->SetX(0);
    player->SetY(0);
    char command = 0;
    while (true) {
        switch (command) {
        case 'w':
            player->moveUp(m_map);
			std::cout << player->GetX() << " " << player->GetY() << "\n";
            break;
        case 'a':
            player->moveLeft(m_map);
            std::cout << player->GetX() << " " << player->GetY() << "\n";
            break;
        case 's':
            player->moveDown(m_map);
            std::cout << player->GetX() << " " << player->GetY() << "\n";
            break;
        case 'd':
            player->moveRight(m_map);
            std::cout << player->GetX() << " " << player->GetY() << "\n";
            break;
        default:
            break;
        }
        for (int i = 0; i < m_map.getMapHeight(); ++i) {
            for (int j = 0; j < m_map.getMapWidth(); ++j) {
                m_map[i][j]->print();
                std::cout << " ";
            }
            std::cout << "\n";
        }
        std::cin >> command;
    }*/

	std::cout << "Rulat cu succes";
	return 0;
}