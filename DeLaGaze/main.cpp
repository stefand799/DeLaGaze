#include "game.h"
//#include <crow.h>
//#include <cpr/cpr.h>     
//#include <sqlite_orm/sqlite_orm.h>
// We comment libraries we don't use to not waste time at compilation when developing


int main() {
	Game game;
	game.Start();
    /*Map m_map;
    m_map.Generate();
    delete m_map[0][0];
    Player* player = new Player(m_map, { 0,0 }, "Player1", 0, false, Direction::South, State::Idle);
    m_map[0][0] = player;
    player->SetX(0);
    player->SetY(0);
    char command = 0;
    while (true) {
        switch (command) {
        case 'w':
            player->MoveUp();
			std::cout << player->GetX() << " " << player->GetY() << "\n";
            break;
        case 'a':
            player->MoveLeft();
            std::cout << player->GetX() << " " << player->GetY() << "\n";
            break;
        case 's':
            player->MoveDown();
            std::cout << player->GetX() << " " << player->GetY() << "\n";
            break;
        case 'd':
            player->MoveRight();
            std::cout << player->GetX() << " " << player->GetY() << "\n";
            break;
        default:
            break;
        }
        for (int i = 0; i < m_map.GetMapHeight(); ++i) {
            for (int j = 0; j < m_map.GetMapWidth(); ++j) {
                if (m_map[i][j]) m_map[i][j]->Print();
                else std::cout << "*";
                std::cout << " ";
            }
            std::cout << "\n";
        }
        std::cin >> command;
    }*/

	std::cout << "Rulat cu succes";
	return 0;
}