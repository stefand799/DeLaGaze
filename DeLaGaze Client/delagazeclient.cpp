#include "delagazeclient.h"


DeLaGazeClient::DeLaGazeClient(QWidget* parent)
    : QMainWindow(parent),
    stackedWidget(new QStackedWidget(this)),
    loginScreen(new LoginScreen(this)),
    mainScreen(new MainScreen(this)),
    playScreen(new PlayScreen(this)),
	upgradesScreen(new UpgradesScreen(this)),
	reqManager(new GameClientReqManager("http://127.0.0.1:18080",this))
{
    ui.setupUi(this);

    this->resize(1280, 960);

    setCentralWidget(stackedWidget);

    stackedWidget->addWidget(loginScreen);
    stackedWidget->addWidget(mainScreen);
    stackedWidget->addWidget(playScreen);
    stackedWidget->addWidget(upgradesScreen);
    initializeConnections();
    stackedWidget->setCurrentWidget(loginScreen);
}

DeLaGazeClient::~DeLaGazeClient()
{
}

void DeLaGazeClient::initializeConnections(){
    connect(loginScreen, &LoginScreen::loginRequest, reqManager, &GameClientReqManager::loginOrCreatePlayer);
    connect(upgradesScreen, &UpgradesScreen::bulletSpeedUpgradeRequest, reqManager, &GameClientReqManager::upgradeBulletSpeed);
    connect(upgradesScreen, &UpgradesScreen::fireRateUpgradeRequest, reqManager, &GameClientReqManager::upgradeFireRate);
	connect(playScreen, &PlayScreen::joinLobbyRequest, reqManager, &GameClientReqManager::joinLobby);

	connect(reqManager, &GameClientReqManager::loginSuccess, this, [&](int id, const std::string& username, int score, int points, int fireRate, bool upgradeBS, const std::string& successMessage)
        {
            upgradesScreen->setPointsAndLevel(points,fireRate);
            upgradesScreen->setScoreAndLevel(score,upgradeBS);
            std::cout << "Login successful for username: " << username << std::endl;
            stackedWidget->setCurrentWidget(mainScreen);
        });
    connect(reqManager, &GameClientReqManager::loginFailed, this, [&](const std::string& errorMessage)
        {
            loginScreen->showServerError(errorMessage);
            stackedWidget->setCurrentWidget(loginScreen);
        });
   

    connect(reqManager, &GameClientReqManager::upgradeBulletSpeedSuccess, this, [&](int score, bool upgradeBS, const std::string& successMessage)
        {
            upgradesScreen->setScoreAndLevel(score, upgradeBS);
            qDebug() << successMessage;
        });
    connect(reqManager, &GameClientReqManager::upgradeBulletSpeedFailed, this, [&](const std::string& errorMessage)
        {
            qDebug() << "Bullet speed FAILED!\n"<<errorMessage;
        });
    connect(reqManager, &GameClientReqManager::upgradeFireRateSuccess, this, [&](int points, int fireRate, const std::string& successMessage)
        {
            upgradesScreen->setPointsAndLevel(points,fireRate);
            qDebug() << successMessage ;
        });
    connect(reqManager, &GameClientReqManager::upgradeFireRateFailed, this, [&](const std::string& errorMessage)
        {
            qDebug() << "FireRate FAILED!\n" << errorMessage;
        });
    connect(reqManager, &GameClientReqManager::joinLobbySuccess, this, [&](const std::string& successMessage){
        if (!lobbyScreen)
        {
            qDebug() << "Created lobby screen!!!!!!!!";
            lobbyScreen = new LobbyScreen(this);
            stackedWidget->addWidget(lobbyScreen);
            qDebug() << "Join Lobby SUCCESS! " << successMessage << "\n";
            stackedWidget->setCurrentWidget(lobbyScreen);

            connect(lobbyScreen, &LobbyScreen::hasGameStartedRequest, reqManager, &GameClientReqManager::checkHasGameStarted);
            connect(reqManager, &GameClientReqManager::hasGameStartedSuccess, lobbyScreen, &LobbyScreen::onGameStart);
            connect(reqManager, &GameClientReqManager::hasGameStartedSuccess, this, [&](const std::string& successMessage)
                {
                    qDebug() << "GAME HAS STARTED! " << successMessage << "\n";
                    if (!gameScreen) {
                        gameScreen = new GameScreen(this);
                        qDebug() << "GAME HAS STARTED! " << successMessage << "\n";
                        stackedWidget->addWidget(gameScreen);  
                    }

                    stackedWidget->setCurrentWidget(gameScreen);
                    delete lobbyScreen;
                    connect(gameScreen, &GameScreen::getGameStateRequest, reqManager, &GameClientReqManager::getGameState);
                    connect(reqManager, &GameClientReqManager::getGameStateSuccess, gameScreen, &GameScreen::onGameStateReceived);
                    connect(reqManager, &GameClientReqManager::getGameStateFailed, this, [&](const std::string& errorMessage)
                        {
                            qDebug() << "Get gamestate FAILED! " << errorMessage << "\n";
                            stackedWidget->setCurrentWidget(mainScreen);
                            delete gameScreen;
                        });

                    connect(gameScreen, &GameScreen::playerMoveRequest, reqManager, &GameClientReqManager::playerMove);
                    connect(reqManager, &GameClientReqManager::playerMoveSuccess, this,[&](const std::string& successMessage)
                    {
                            qDebug() << "Moved successfully! " << successMessage << "\n";
                    });
                    connect(reqManager, &GameClientReqManager::playerMoveFailed, this, [&](const std::string& errorMessage)
                        {
                            qDebug() << "Move FAILED! " << errorMessage << "\n";
                        });

                    connect(gameScreen, &GameScreen::playerFaceRequest, reqManager, &GameClientReqManager::playerFace);
                    connect(reqManager, &GameClientReqManager::playerFaceSuccess, this, [&](const std::string& successMessage)
                        {
                            qDebug() << "Facing successful! " << successMessage << "\n";
                        });
                    connect(reqManager, &GameClientReqManager::playerFaceFailed, this, [&](const std::string& errorMessage)
                        {
                            qDebug() << "Facing FAILED! " << errorMessage << "\n";
                        });

                    connect(gameScreen, &GameScreen::playerShootRequest, reqManager, &GameClientReqManager::playerShoot);
                    connect(reqManager, &GameClientReqManager::playerShootSuccess, this, [&](const std::string& successMessage)
                        {
                            qDebug() << "SHOT successfully! " << successMessage << "\n";
                        });
                    connect(reqManager, &GameClientReqManager::playerShootFailed, this, [&](const std::string& errorMessage)
                        {
                            qDebug() << "SHOT FAILED! " << errorMessage << "\n";
                        });

                });
            connect(reqManager, &GameClientReqManager::hasGameStartedFailed, this, [&](const std::string& errorMessage)
                {
                    qDebug() << errorMessage;
                });
        }
        });
    connect(reqManager, &GameClientReqManager::joinLobbyFailed, this, [&](const std::string& errorMessage)
        {
            qDebug() << "Join Lobby FAILED! " << errorMessage << "\n";
        });
    


	connect(mainScreen, &MainScreen::selectedScreen, this, [&](MainScreen::Screen screen) {
        switch (screen)
        {
       
        case MainScreen::Screen::UpgradesScreen:
            stackedWidget->setCurrentWidget(upgradesScreen);
            break;
        case MainScreen::Screen::PlayScreen :
            stackedWidget->setCurrentWidget(playScreen);
            break;
        case MainScreen::Screen::LoginScreen:
            stackedWidget->setCurrentWidget(loginScreen);
            break;
        }
        });
    connectBackButtonToMainScreen(playScreen, stackedWidget, mainScreen);
    connectBackButtonToMainScreen(upgradesScreen, stackedWidget, mainScreen);
}
