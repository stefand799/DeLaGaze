#include "delagazeclient.h"

DeLaGazeClient::DeLaGazeClient(QWidget* parent)
    : QMainWindow(parent),
    stackedWidget(new QStackedWidget(this)),
    loginScreen(new LoginScreen(this)),
    mainScreen(new MainScreen(this)),
    playScreen(new PlayScreen(this)),
	upgradesScreen(new UpgradesScreen(this)),
	lobbyScreen(new LobbyScreen(this)),
	reqManager(new GameClientReqManager("http://localhost:18080",this))
{
    ui.setupUi(this);

    this->resize(1920, 1080);
    //this->setFixedSize(1920, 1080);

    setCentralWidget(stackedWidget);

    stackedWidget->addWidget(loginScreen);
    stackedWidget->addWidget(mainScreen);
    stackedWidget->addWidget(playScreen);
    stackedWidget->addWidget(upgradesScreen);
    stackedWidget->addWidget(lobbyScreen);
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
    connect(playScreen, &PlayScreen::joinLobbyRequest, reqManager, &GameClientReqManager::joinLobby);

	connect(reqManager, &GameClientReqManager::loginSuccess, this, [&](const std::string& username, int score, int points, int fireRate, bool upgradeBS)
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
    connect(reqManager, &GameClientReqManager::addPlayerFailed, this, [&](const std::string& errorMessage)
        {
        /*lmbd function for lambdas that identically handle SERVER ERRORS*/
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
    connect(reqManager, &GameClientReqManager::joinLobbySuccess, this, [&](const std::string& successMessage)
        {   stackedWidget->setCurrentWidget(lobbyScreen);
            qDebug() << "Join Lobby SUCCESS! " << successMessage << "\n";
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
    connect(lobbyScreen, &LobbyScreen::backButtonClicked, this, [&]()
        {
        /*TODO: REQUEST FOR LEAVING THE LOBBY!!*/
            stackedWidget->setCurrentWidget(playScreen);
        });
}
