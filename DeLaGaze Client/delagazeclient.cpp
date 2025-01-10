#include "delagazeclient.h"
DeLaGazeClient::DeLaGazeClient(QWidget* parent)
    : QMainWindow(parent),
    stackedWidget(new QStackedWidget(this)),
    loginScreen(new LoginScreen(this)),
    mainScreen(new MainScreen(this)),
    playScreen(new PlayScreen(this)),
	upgradesScreen(new UpgradesScreen(this)),
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
    connect(reqManager, &GameClientReqManager::upgradeBulletSpeedFailed, this, [&]()
        {
            qDebug() << "Bullet speed FAILED!\n";
        });
    connect(reqManager, &GameClientReqManager::upgradeFireRateSuccess, this, [&]()
        {
            qDebug() << "FireRate UPGRADED!\n";
        });
    connect(reqManager, &GameClientReqManager::upgradeFireRateFailed, this, [&]()
        {
            qDebug() << "FireRate FAILED!\n";
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
    connect(playScreen, &PlayScreen::selectedScreen, this, [&](PlayScreen::Screen screen) {
        switch (screen)
        {
        case PlayScreen::Screen::MainScreen:
            stackedWidget->setCurrentWidget(mainScreen);
            /*TODO: Implement the other screens, for the gamemodes, make the network calls in their constructors*/
            break;
        }
        });
    connect(upgradesScreen, &UpgradesScreen::backButtonClicked, this, [&]()
        {
            stackedWidget->setCurrentWidget(mainScreen);
        });
}
