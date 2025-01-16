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
#include "GameMode.h"



class Game {
public:
	Game();
	~Game();
	bool AddPlayers(const std::vector<std::shared_ptr<Player>>& players);
	void RemovePlayer(const Player& player);
	void Start();

	Map& GetMap();
	std::shared_ptr<Player> GetPlayerByName(const std::string& username);
	std::vector<std::shared_ptr<Bullet>>& GetBullets() {
		return m_bullets;
	}
	void SetGameMode(GameMode gameMode);
	crow::json::wvalue GameStateToJson();
private:
	//Usings
	using Clock = std::chrono::high_resolution_clock;
	using Nsec = std::chrono::nanoseconds;
	using fSecDur = std::chrono::duration<float>;


	//Methods
	crow::json::wvalue BulletsToJson();
	crow::json::wvalue PlayersToJson();
	crow::json::wvalue MapToJson();

	void Run();
	void CheckDeathmatchCondition();
	bool CheckEndCondition();
	void HandleEndOfGameActions();
	
	void Update();
	void HandleCollisions();
	void HandleBulletToWallCollisions(std::shared_ptr<Bullet>& bullet);
	void HandleBulletToBorderCollisions(std::shared_ptr<Bullet>& bullet);
	void HandleBulletToBulletCollisions(std::vector<std::shared_ptr<Bullet>>::iterator& bulletIterator);
	void HandleBulletToPlayerCollisions(std::shared_ptr<Bullet>& bullet);

	void RemoveDestroyedObjects();

	
	//void GetPlayerInputs();

	//Constants
	const uint16_t kMaxFps = 30;

	const std::chrono::seconds kTimeUntilDeathmatch = std::chrono::seconds(177);


	Nsec m_shortestSleepTime;

	

	//Atributes
	Map m_map;
	std::vector<std::shared_ptr<Player>> m_players;
	std::vector<std::shared_ptr<Bullet>> m_bullets;
	GameMode m_mode;
	
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
	float m_deltaTime;
	Clock::time_point m_lastFrameTime;

	Nsec m_targetFrameDuration;

	bool m_isRunning;


	Clock::time_point m_startGameTime;
	bool m_hasDeathmatchStarted;
	
	
	std::queue<std::pair<std::shared_ptr<Player>,char>> m_playerInputs;

	std::vector<std::vector<uint8_t>> m_teamLeaderboard;

};