#pragma once
#include <QObject>
#include <iostream>

class GameClientReqManager :public QObject
{
	Q_OBJECT
public:
	
	GameClientReqManager(const std::string& serverUrl, QObject* parent = nullptr);
	void loginOrCreatePlayer(const std::string& username);
	void upgradeFireRate();
	void upgradeBulletSpeed();
	void joinLobby(const std::string& gameMode);
	void checkHasGameStarted();
	void getGameState();
	void playerMove(const std::string& direction);
	void playerFace(const std::string& direction);
	void playerShoot();
signals:
	void loginSuccess(int id,const std::string& username, int score, int points, int fireRate, bool upgradeBS, const std::string& successMessage);
	void loginFailed(const std::string& errorMessage);
	void upgradeFireRateSuccess(int points,int fireRate,const std::string& successMessage);
	void upgradeFireRateFailed(const std::string& errorMessage);
	void upgradeBulletSpeedSuccess(int score, bool upgradeBS, const std::string& successMessage);
	void upgradeBulletSpeedFailed(const std::string& errorMessage);
	void joinLobbySuccess(const std::string& successMessage);
	void joinLobbyFailed(const std::string& errorMessage);
	void hasGameStartedSuccess(const std::string& successMessage);
	void hasGameStartedFailed(const std::string& errorMessage);
	void getGameStateSuccess(const std::string& successMessage);
	void getGameStateFailed(const std::string& errorMessage);
	void playerMoveSuccess(const std::string& successMessage);
	void playerMoveFailed(const std::string& errorMessage);
	void playerFaceSuccess(const std::string& successMessage);
	void playerFaceFailed(const std::string& errorMessage);
	void playerShootSuccess(const std::string& successMessage);
	void playerShootFailed(const std::string& errorMessage);
private:
	std::string serverUrl;
	std::string m_username;
};
