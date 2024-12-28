#pragma once

#include "Object.h"
#include "UnbreakableBlock.h"
#include "BreakableBlock.h"
#include "Pathway.h"
#include "BombTrapBlock.h"
#include "Player.h"
#include "Bullet.h"
#include "Map.h"
#include <vector>
#include <unordered_set>
#include <cstdint>
#include <chrono>
#include <thread>

#define DEBUG

//These are included only for the command-line UI for testing and will be removed after ui can be done in QT
#include <WinSock2.h>
#include<Windows.h>
#include <conio.h>



class Game {
public:
	Game();
	~Game();
	void AddPlayer(const Player& player);
	void RemovePlayer(const Player& player);


	Map& GetMap();
	void Start();



private:
	//Usings
	using Clock = std::chrono::high_resolution_clock;
	using Nsec = std::chrono::nanoseconds;
	using fSecDur = std::chrono::duration<float>; //floating point second


	//Methods
	crow::json::wvalue BulletsToJson();

	void Run();
	bool CheckEndCondition();
	void HandleEndOfGameActions();
	
	void Update();
	void HandleCollisions();
	void HandleBulletToWallCollisions(std::shared_ptr<Bullet>& bullet);
	void HandleBulletToBorderCollisions(std::shared_ptr<Bullet>& bullet);
	void HandleBulletToBulletCollisions(std::vector<std::shared_ptr<Bullet>>::iterator& bulletIterator);
	void HandleBulletToPlayerCollisions(std::shared_ptr<Bullet>& bullet);

	void RemoveDestroyedObjects();

	void GetPlayerInputs();

	//Constants
	const uint16_t m_maxFps = 30; //for windows as of now the maximum fps achievable with the hybrid sleep and busy-waiting method in use is 64hz


	Nsec m_shortestSleepTime;

	

	//Atributes
	Map m_map;
	std::vector<std::shared_ptr<Player>> m_players;
	std::vector<std::shared_ptr<Bullet>> m_bullets; //We could probably use shared_ptr so the bullets get deleted immediately when they go out of scope
	//std::vector<Object*> m_markedForDestruction; //Object*, because currently I have no clue if we're going to alter additional logic and would be optimal not to immediately delete them when they get out of scope
	
	struct ObjectCollision {
		ObjectCollision(std::shared_ptr<Object> obj1, std::shared_ptr<Object> obj2, float collisionTime) :
			first{ obj1 },
			second{ obj2 },
			time{ collisionTime },
			isBorderCollision{ false }
		{}
		ObjectCollision(std::tuple<std::shared_ptr<Object>,std::shared_ptr<Object>,float>&& values) :
			first{ std::get<0>(values)},
			second{ std::get<1>(values) },
			time{ std::get<2>(values) },
			isBorderCollision{ false }
		{}
		ObjectCollision(std::shared_ptr<Object> obj, float collisionTime) :
			first{ obj },
			second{ nullptr },
			time{ collisionTime },
			isBorderCollision{ true }
		{}

		std::shared_ptr<Object> first;
		std::shared_ptr<Object> second;
		float time;
		bool isBorderCollision;
		bool operator<(const ObjectCollision& other) const {
			return this->time < other.time;
		}
	};

	std::priority_queue<ObjectCollision> m_collisions;
	float m_deltaTime; //The time from the last frame
	Clock::time_point m_lastFrameTime;

	Nsec m_targetFrameDuration;

	bool m_isRunning;
	
	std::queue<std::pair<std::shared_ptr<Player>,char>> m_playerInputs;

	std::vector<std::vector<uint8_t>> m_teamLeaderboard;

	//DEBUG METHODS:
	
	void __DEBUG_PRINT_MAP__();

};