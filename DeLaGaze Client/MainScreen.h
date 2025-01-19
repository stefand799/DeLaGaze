#pragma once
#include "MiauScreen.h"
#include "qwidget.h"
#include "qpushbutton.h"

class MainScreen:public MiauScreen{
	Q_OBJECT
public:
	MainScreen(QWidget* parent = nullptr);
	enum class Screen
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
};

