#include "GameClientReqManager.h"
#include <cpr/cpr.h>
#include <crow/json.h>

GameClientReqManager::GameClientReqManager(const std::string& serverUrl, QObject* parent)
	:QObject(parent),
	serverUrl(serverUrl)
{}

void GameClientReqManager::loginOrCreatePlayer(const std::string& username) {
	auto response = cpr::Get(cpr::Url{ serverUrl + "/login/" + username });
	if (response.status_code == 200 || response.status_code == 201)
	{
		auto jsonResponse = crow::json::load(response.text);
		if (!jsonResponse)
		{
			emit loginFailed("Invalid JSON from server.");
			return;
		}
		m_username = username;
		emit loginSuccess(
			jsonResponse["id"].i(),
			jsonResponse["username"].s(),
			jsonResponse["score"].i(),
			jsonResponse["points"].i(),
			jsonResponse["firerate"].i(),
			jsonResponse["upgrade_bs"].b(),
			response.text
		);
	}
	else
		emit loginFailed(response.text);
}

void GameClientReqManager::upgradeFireRate()
{
	auto response = cpr::Post(
		cpr::Url{ serverUrl + "/players/update_firerate/" + m_username },
		cpr::Header{ {"Content-Type","application/json"} }
	);

	if (response.status_code == 200)
	{
		auto jsonResponse = crow::json::load(response.text);
		int points = jsonResponse["points"].i();
		int fireRate = jsonResponse["fireRate"].i();
		std::string message = jsonResponse["message"].s();
		emit upgradeFireRateSuccess(points,fireRate,message);
	}
		else
		emit upgradeFireRateFailed(response.text);
}

void GameClientReqManager::upgradeBulletSpeed()
{
	auto response = cpr::Post(
		cpr::Url{ serverUrl + "/players/update_bullet_speed/"+m_username },
		cpr::Header{ {"Content-Type","application/json"} }
	);

	if (response.status_code == 200)
	{
		auto jsonResponse = crow::json::load(response.text);
		int score = jsonResponse["score"].i();
		int upgradeBS = jsonResponse["upgrade_bs"].b();
		std::string message = jsonResponse["message"].s();
		emit upgradeBulletSpeedSuccess(score,upgradeBS,message);
	}
	else
		emit upgradeBulletSpeedFailed(response.text);
}

void GameClientReqManager::joinLobby(const std::string& gameMode)
{
	auto response = cpr::Post(
		cpr::Url{ serverUrl + "/join/" + gameMode + "/" + m_username },
		cpr::Header{ {"Content-Type","application/json"} }
	);

	if (response.status_code == 200)
		emit joinLobbySuccess(response.text);
	else
		emit joinLobbyFailed(response.text);
}

void GameClientReqManager::checkHasGameStarted()
{
	auto response = cpr::Get(
		cpr::Url{ serverUrl + "/gamestarted/" + m_username },
		cpr::Header{ {"Content-Type","application/json"} }
	);
	if (response.status_code == 200)
		emit hasGameStartedSuccess(response.text);
	else
		emit hasGameStartedFailed(response.text);
}

void GameClientReqManager::getGameState()
{
	auto response = cpr::Get(
		cpr::Url{ serverUrl + "/getgamestate/" + m_username },
		cpr::Header{ {"Content-Type","application/json"} }
	);

	if (response.status_code == 200)
		emit getGameStateSuccess(response.text);
	else
		emit getGameStateFailed(response.text);
}

void GameClientReqManager::playerMove(const std::string& direction)
{
	auto response = cpr::Post(
		cpr::Url{ serverUrl + "/move/" + m_username + "/" + direction },
		cpr::Header{ {"Content-Type","application/json"} }
	);

	if (response.status_code == 200)
		emit playerMoveSuccess(response.text);
	else
		emit playerMoveFailed(response.text);
}

void GameClientReqManager::playerFace(const std::string& direction)
{
	auto response = cpr::Post(
		cpr::Url{ serverUrl + "/face/" + m_username + "/" + direction },
		cpr::Header{ {"Content-Type","application/json"} }
	);
	if (response.status_code == 200)
		emit playerFaceSuccess(response.text);
	else
		emit playerFaceFailed(response.text);
}

void GameClientReqManager::playerShoot()
{
	auto response = cpr::Post(
		cpr::Url{ serverUrl + "/shoot/" + m_username },
		cpr::Header{ {"Content-Type","application/json"} }
	);

}
