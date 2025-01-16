#include "GameClientReqManager.h"
#include <cpr/cpr.h>
#include <crow/json.h>

GameClientReqManager::GameClientReqManager(const std::string& serverUrl, QObject* parent)
	:QObject(parent),
	serverUrl(serverUrl)
{}

void GameClientReqManager::loginOrCreatePlayer(const std::string& username) {
	auto response = cpr::Get(cpr::Url{ serverUrl + "/login/" + username });
	if (response.status_code == 200)
	{
		auto jsonResponse = crow::json::load(response.text);
		if (!jsonResponse)
		{
			emit loginFailed("Invalid JSON from server.");
			return;
		}
		m_username = username;
		emit loginSuccess(
			//jsonResponse["id"].i(),
			jsonResponse["username"].s(),
			jsonResponse["score"].i(),
			jsonResponse["points"].i(),
			jsonResponse["firerate"].i(),
			jsonResponse["upgrade_bs"].b(),
			response.text
		);
	}
	else if (response.status_code == 400)
	{

		crow::json::wvalue newPlayerJson;
		newPlayerJson["username"] = username;

		auto createResponse = cpr::Post(
			cpr::Url{ serverUrl + "/players/add" },
			cpr::Body{ newPlayerJson.dump() },
			cpr::Header{ {"Content-Type","application/json"} }
		);
		/*TODO: After refactoring, send the json instead of default values; also, think of the id*/
		if (createResponse.status_code == 201)
		{
			m_username = username;
			emit loginSuccess(username, 0, 0, 1, false, response.text);
		}
		else
		{
			emit addPlayerFailed(createResponse.text);
		}
	}
	else
	{
		emit loginFailed(response.text);
	}
}

void GameClientReqManager::upgradeFireRate()
{
	auto response = cpr::Post(
		cpr::Url{ serverUrl + "/players/update_firerate/" + m_username },
		cpr::Header{ {"Content-Type","application/json"} }
	);

	if (response.status_code == 200)
		emit upgradeFireRateSuccess(response.text);
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
		emit upgradeBulletSpeedSuccess(response.text);
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
