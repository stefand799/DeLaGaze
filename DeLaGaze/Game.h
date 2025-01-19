#pragma once
#include "Object.h"
#include "UnbreakableBlock.h"
#include "BreakableBlock.h"
#include "Pathway.h"
#include "BombTrapBlock.h"
#include "Player.h"
#include "Bullet.h"
#include "Map.h"
#include "ObjectCollision.h"
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
	bool AddPlayers(const std::vector<std::shared_ptr<Player>>& players, GameMode gm);
	void RemovePlayer(const Player& player);
	void Start();

	Map& GetMap();
	std::shared_ptr<Player> GetPlayerByName(const std::string& username);
	std::vector<std::shared_ptr<Bullet>>& GetBullets() {
		return m_bullets;
	}
	void SetGameMode(GameMode gameMode);
	crow::json::wvalue GameStateToJson();

	bool IsRunning() { return m_isRunning; };
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

	
	//Constants
	const uint16_t kMaxFps = 60;

	const std::chrono::seconds kTimeUntilDeathmatch = std::chrono::seconds(175);


	Nsec m_shortestSleepTime;


	//Atributes
	Map m_map;
	std::vector<std::shared_ptr<Player>> m_players;
	std::vector<std::shared_ptr<Bullet>> m_bullets;
	GameMode m_mode;

	std::priority_queue<ObjectCollision> m_collisions;
	float m_deltaTime;
	Clock::time_point m_lastFrameTime;

	Nsec m_targetFrameDuration;

	bool m_isRunning = false;


	Clock::time_point m_startGameTime;
	bool m_hasDeathmatchStarted = false;
	
	
	std::queue<std::pair<std::shared_ptr<Player>,char>> m_playerInputs;

	std::vector<std::vector<uint8_t>> m_teamLeaderboard;
	std::unordered_set<uint8_t> m_eliminatedTeams;

};