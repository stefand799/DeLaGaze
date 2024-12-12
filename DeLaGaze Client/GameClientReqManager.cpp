#include "GameClientReqManager.h"
#include <cpr/cpr.h>
#include <crow/json.h>

GameClientReqManager::GameClientReqManager(const std::string& serverUrl, QObject* parent)
	:QObject(parent),
	serverUrl(serverUrl)
{}

void GameClientReqManager::loginOrCreatePlayer(const std::string& username) {
	auto response = cpr::Get(cpr::Url{ serverUrl + "/login/" + username });
	if (response.status_code = 200)
	{
		auto jsonResponse = crow::json::load(response.text);
		if (!jsonResponse)
		{
			emit loginFailed("Invalid JSON from server.");
			return;
		}
		emit loginSuccess(
			jsonResponse["username"].s(),
			jsonResponse["score"].i(),
			jsonResponse["points"].i(),
			jsonResponse["firerate"].i(),
			jsonResponse["upgrade_bs"].b()
		);
	}
	else if (response.status_code = 404)
	{
		crow::json::wvalue newPlayerJson;
		newPlayerJson["username"] = username;

		auto createResponse = cpr::Post(
			cpr::Url{ serverUrl + "/addPlayer" },
			cpr::Body{ newPlayerJson.dump() },
			cpr::Header{ {"Content-Type","application/json"} }
		);

		if (createResponse.status_code == 201)
		{
			emit loginSuccess(username, 0, 0, 1, false);
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
