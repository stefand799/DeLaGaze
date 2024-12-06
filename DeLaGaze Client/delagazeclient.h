#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_delagazeclient.h"
#include "qstackedwidget.h"
#include "MainScreen.h"
#include "LoginScreen.h"
#include "PlayScreen.h"
#include "UpgradesScreen.h"

class DeLaGazeClient : public QMainWindow
{
    Q_OBJECT

public:
    DeLaGazeClient(QWidget *parent = nullptr);
    ~DeLaGazeClient();

private:
    Ui::DeLaGazeClientClass ui;
    QStackedWidget* stackedWidget;
    LoginScreen* loginScreen;
    MainScreen* mainScreen;
    PlayScreen* playScreen;
    UpgradesScreen* upgradesScreen;

    void initializeConnections();
};
