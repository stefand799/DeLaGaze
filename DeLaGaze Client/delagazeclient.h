#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_delagazeclient.h"
#include "qstackedwidget.h"
#include "MainScreen.h"
#include "LoginScreen.h"

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

    void initializeConnections();
};
