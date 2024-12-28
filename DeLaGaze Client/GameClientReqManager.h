#pragma once
#include <QObject>
#include <iostream>

class GameClientReqManager :public QObject
{
	Q_OBJECT
public:
	/*TODO: replace the signals emitted by each screen, mainly those for navigation, with the proper server
	 * induced ones, ALSO including signals inside of them!
	 */
	GameClientReqManager(const std::string& serverUrl, QObject* parent = nullptr);
	void loginOrCreatePlayer(const std::string& username);
	void upgradeFireRate();
	void upgradeBulletSpeed();
signals:
	void loginSuccess(const std::string& username, int score, int points, int fireRate, bool upgradeBS);
	void loginFailed(const std::string& errorMessage);
	void addPlayerFailed(const std::string& errorMessage);
	void upgradeFireRateSuccess();
	void upgradeFireRateFailed();
	void upgradeBulletSpeedSuccess();
	void upgradeBulletSpeedFailed();
private:
	std::string serverUrl;
};
