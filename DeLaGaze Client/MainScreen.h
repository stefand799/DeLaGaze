#pragma once
#include "qwidget.h"
#include "qpushbutton.h"

class MainScreen:public QWidget{
	Q_OBJECT
public:
	MainScreen(QWidget* parent = nullptr);
private:
	QPushButton* logoutButton;
	QPushButton* upgradesButton;
	QPushButton* playButton;
};

