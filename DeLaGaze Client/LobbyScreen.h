#pragma once

#include <QWidget>

#include "MiauScreen.h"
#include "qlabel.h"
#include "qpushbutton.h"

class LobbyScreen : public MiauScreen
{
    Q_OBJECT

public:
    LobbyScreen(QWidget* parent = nullptr);
    ~LobbyScreen();

signals:  
    void backButtonClicked();

private:
    QLabel* waitingLabel;
    QPushButton* backButton;
};