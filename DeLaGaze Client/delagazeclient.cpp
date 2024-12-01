#include "delagazeclient.h"

DeLaGazeClient::DeLaGazeClient(QWidget* parent)
    : QMainWindow(parent),
    stackedWidget(new QStackedWidget(this)),
    loginScreen(new LoginScreen(this)),
    mainScreen(new MainScreen(this)),
    playScreen(new PlayScreen(this))
{
    ui.setupUi(this);

    setCentralWidget(stackedWidget);

    stackedWidget->addWidget(loginScreen);
    stackedWidget->addWidget(mainScreen);
    stackedWidget->addWidget(playScreen);
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
       /* case MainScreen::PlayScreen:
            stackedWidget->setCurrentWidget(playScreen);
            break;
        case MainScreen::UpgradesScreen:
            stackedWidget->setCurrentWidget(upgradesScreen);
            break;*/
        case MainScreen::PlayScreen:
            stackedWidget->setCurrentWidget(playScreen);
            break;
        case MainScreen::LoginScreen:
            stackedWidget->setCurrentWidget(loginScreen);
            break;
        }
        });
}
