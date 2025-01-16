#pragma once

#include <QWidget>

#include "MiauScreen.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qtimer.h"

class LobbyScreen : public MiauScreen
{
    Q_OBJECT

public:
    LobbyScreen(QWidget* parent = nullptr);
    ~LobbyScreen();

signals:  
    void backButtonClicked();
    void hasGameStartedRequest();
public slots:
    void onGameStart();
private:
    QTimer* hasGameStartedTimer;
    QLabel* waitingLabel;
    QPushButton* backButton;
};