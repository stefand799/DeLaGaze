#include "delagazeclient.h"


DeLaGazeClient::DeLaGazeClient(QWidget* parent)
    : QMainWindow(parent),
    stackedWidget(new QStackedWidget(this)),
    loginScreen(new LoginScreen(this)),
    mainScreen(new MainScreen(this)),
    playScreen(new PlayScreen(this)),
	upgradesScreen(new UpgradesScreen(this)),
	//lobbyScreen(new LobbyScreen(this)),
	//gameScreen(new GameScreen(this)),
	reqManager(new GameClientReqManager("http://127.0.0.1:18080",this))
{
    ui.setupUi(this);

    this->resize(1920, 1080);
    //this->setFixedSize(1920, 1080);

    setCentralWidget(stackedWidget);

    stackedWidget->addWidget(loginScreen);
    stackedWidget->addWidget(mainScreen);
    stackedWidget->addWidget(playScreen);
    stackedWidget->addWidget(upgradesScreen);
    //stackedWidget->addWidget(lobbyScreen);
    //stackedWidget->addWidget(gameScreen);
    initializeConnections();
    stackedWidget->setCurrentWidget(loginScreen);
}

DeLaGazeClient::~DeLaGazeClient()
{
    delete reqManager;
}

void DeLaGazeClient::initializeConnections(){
    //server components
    connect(loginScreen, &LoginScreen::loginRequest, reqManager, &GameClientReqManager::loginOrCreatePlayer);
    connect(upgradesScreen, &UpgradesScreen::bulletSpeedUpgradeRequest, reqManager, &GameClientReqManager::upgradeBulletSpeed);
    connect(upgradesScreen, &UpgradesScreen::fireRateUpgradeRequest, reqManager, &GameClientReqManager::upgradeFireRate);
    //connect(lobbyScreen, &LobbyScreen::hasGameStartedRequest, reqManager, &GameClientReqManager::checkHasGameStarted);
	connect(playScreen, &PlayScreen::joinLobbyRequest, reqManager, &GameClientReqManager::joinLobby);
    //connect(gameScreen, &GameScreen::getGameStateRequest, reqManager, &GameClientReqManager::getGameState);

	connect(reqManager, &GameClientReqManager::loginSuccess, this, [&](int id, const std::string& username, int score, int points, int fireRate, bool upgradeBS, const std::string& successMessage)
        {
            std::cout << "Login successful for username: " << username << std::endl;
        /*TODO: SET functions for screens which will display them (upgrades, and maybe main too)*/
            stackedWidget->setCurrentWidget(mainScreen);
        });
    connect(reqManager, &GameClientReqManager::loginFailed, this, [&](const std::string& errorMessage)
        {
            loginScreen->showServerError(errorMessage);
            stackedWidget->setCurrentWidget(loginScreen);
        });
   

    connect(reqManager, &GameClientReqManager::upgradeBulletSpeedSuccess, this, [&]()
        {
        /*TODO: display UI changes, accordingly. Also add the setters for the displayed values when the player is logged in*/
            qDebug() << "Bullet speed UPGRADED!\n";
        });
    connect(reqManager, &GameClientReqManager::upgradeBulletSpeedFailed, this, [&](const std::string& errorMessage)
        {
            qDebug() << "Bullet speed FAILED!\n"<<errorMessage;
        });
    connect(reqManager, &GameClientReqManager::upgradeFireRateSuccess, this, [&]()
        {
            qDebug() << "FireRate UPGRADED!\n";
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
            connect(lobbyScreen, &LobbyScreen::backButtonClicked, this, [&]()
                {
                    /*TODO: REQUEST FOR LEAVING THE LOBBY!!*/
                    stackedWidget->setCurrentWidget(playScreen);
                });
            
            connect(reqManager, &GameClientReqManager::hasGameStartedSuccess, lobbyScreen, &LobbyScreen::onGameStart);
            connect(reqManager, &GameClientReqManager::hasGameStartedSuccess, this, [&](const std::string& successMessage)
                {
                    qDebug() << "GAME HAS STARTED! " << successMessage << "\n";
                    if (!gameScreen) {
                        gameScreen = new GameScreen(this);
                        qDebug() << "GAME HAS STARTED! " << successMessage << "\n";
                        // Dynamically create the GameScreen
                        stackedWidget->addWidget(gameScreen);  // Add it to the stacked widget
                    }

                    stackedWidget->setCurrentWidget(gameScreen);
                    connect(gameScreen, &GameScreen::getGameStateRequest, reqManager, &GameClientReqManager::getGameState);
                    connect(reqManager, &GameClientReqManager::getGameStateSuccess, gameScreen, &GameScreen::onGameStateReceived);
                    connect(reqManager, &GameClientReqManager::getGameStateFailed, this, [&](const std::string& errorMessage)
                        {
                            qDebug() << "Get gamestate FAILED! " << errorMessage << "\n";
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
    

    


    //UI components
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
    connect(playScreen, &PlayScreen::backButtonClicked, this, [&]() {
        stackedWidget->setCurrentWidget(mainScreen);
        });
    connect(upgradesScreen, &UpgradesScreen::backButtonClicked, this, [&]()
        {
            stackedWidget->setCurrentWidget(mainScreen);
        });
    
}
