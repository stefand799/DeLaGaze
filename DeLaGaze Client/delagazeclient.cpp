#include "delagazeclient.h"

DeLaGazeClient::DeLaGazeClient(QWidget* parent)
    : QMainWindow(parent),
    stackedWidget(new QStackedWidget(this)),
    loginScreen(new LoginScreen(this)),
    mainScreen(new MainScreen(this)),
    playScreen(new PlayScreen(this)),
	upgradesScreen(new UpgradesScreen(this))
{
    ui.setupUi(this);

    setCentralWidget(stackedWidget);

    stackedWidget->addWidget(loginScreen);
    stackedWidget->addWidget(mainScreen);
    stackedWidget->addWidget(playScreen);
    stackedWidget->addWidget(upgradesScreen);
    initializeConnections();
    stackedWidget->setCurrentWidget(loginScreen);
}

DeLaGazeClient::~DeLaGazeClient()
{}

void DeLaGazeClient::initializeConnections(){
    connect(loginScreen, &LoginScreen::loginSuccessful, this, [&]() {
        stackedWidget->setCurrentWidget(mainScreen);
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
    connect(playScreen, &PlayScreen::selectedScreen, this, [&](PlayScreen::Screen screen) {
        switch (screen)
        {
        case PlayScreen::Screen::MainScreen:
            stackedWidget->setCurrentWidget(mainScreen);
            /*TODO: Implement the other screens, for the gamemodes, make the network calls in their constructors*/
            break;
        }
        });
    //connect(upgradeScreen,&,this,)
}
