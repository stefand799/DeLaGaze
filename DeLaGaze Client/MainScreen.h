#pragma once
#include "qwidget.h"
#include "qpushbutton.h"

class MainScreen:public QWidget{
	Q_OBJECT
public:
	MainScreen(QWidget* parent = nullptr);
	enum Screen
	{
		PlayScreen,
		UpgradesScreen,
		OptionsScreen,
		LoginScreen
	};
signals:
	void selectedScreen(MainScreen::Screen);
private slots:
	void buttonClicked();
private:
	QPushButton* upgradesButton;
	QPushButton* playButton;
	QPushButton* logoutButton;
	QPushButton* optionsButton;
};

