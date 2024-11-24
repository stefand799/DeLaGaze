#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_delagazeclient.h"

class DeLaGazeClient : public QMainWindow
{
    Q_OBJECT

public:
    DeLaGazeClient(QWidget *parent = nullptr);
    ~DeLaGazeClient();

private:
    Ui::DeLaGazeClientClass ui;
};
