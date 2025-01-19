#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_delagazeclient.h"
#include "qstackedwidget.h"
#include "MainScreen.h"
#include "LoginScreen.h"
#include "PlayScreen.h"
#include "UpgradesScreen.h"
#include "LobbyScreen.h"
#include "GameScreen.h"
#include "GameClientReqManager.h"

class DeLaGazeClient : public QMainWindow
{
    Q_OBJECT

public:
    DeLaGazeClient(QWidget *parent = nullptr);
    ~DeLaGazeClient();
    template<typename Screen>
    void connectBackButtonToMainScreen(Screen* screen, QStackedWidget* stackedWidget, QWidget* mainScreen) {
        connect(screen, &Screen::backButtonClicked, stackedWidget, [stackedWidget, mainScreen]() {
            stackedWidget->setCurrentWidget(mainScreen);
            });
    }

private:
    Ui::DeLaGazeClientClass ui;
    QStackedWidget* stackedWidget;
    LoginScreen* loginScreen;
    MainScreen* mainScreen;
    PlayScreen* playScreen;
    UpgradesScreen* upgradesScreen;
    LobbyScreen* lobbyScreen;
    GameScreen* gameScreen;
    GameClientReqManager* reqManager;

    void initializeConnections();
};
