#include "delagazeclient.h"

DeLaGazeClient::DeLaGazeClient(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    loginScreen = new LoginScreen(this);

    stackedWidget->addWidget(loginScreen);
    stackedWidget->setCurrentWidget(loginScreen);
}

DeLaGazeClient::~DeLaGazeClient()
{}
