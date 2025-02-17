#include "MainScreen.h"
#include "qboxlayout.h"

MainScreen::MainScreen(QWidget* parent)
	:MiauScreen(":/DeLaGazeClient/images/miau_main.png",parent),
	 playButton(new QPushButton("Play",this)),
	 upgradesButton(new QPushButton("Upgrades",this)),
	 logoutButton(new QPushButton("Logout",this)){

	auto groupLayout = new QHBoxLayout();
	QList<QPushButton*> buttons = { playButton,upgradesButton,logoutButton };
	QFont buttonFont;
	buttonFont.setPointSize(20);
	for (auto button : buttons) {
		button->setFixedHeight(70);
		button->setFont(buttonFont);
		groupLayout->addWidget(button);
		connect(button, &QPushButton::clicked, this, &MainScreen::buttonClicked);

	}
	
	auto layout = new QVBoxLayout(this);
	layout->addStretch();
	layout->addLayout(groupLayout);
}

void MainScreen::buttonClicked(){
	QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
	if (clickedButton == nullptr) {
		qWarning() << "Sender does not belong to the known buttons!";
	} else if (clickedButton==playButton){
		emit selectedScreen(Screen::PlayScreen);
	}else if (clickedButton==upgradesButton){
		emit selectedScreen(Screen::UpgradesScreen);
	}else if (clickedButton==logoutButton){
		emit selectedScreen(Screen::LoginScreen);
	}
}

