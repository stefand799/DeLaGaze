#include "MainScreen.h"
#include "qboxlayout.h"

MainScreen::MainScreen(QWidget* parent)
	:QWidget(parent),
	 playButton(new QPushButton("Play",this)),
	 upgradesButton(new QPushButton("Upgrades",this)),
	 optionsButton(new QPushButton("Options",this)),
	 logoutButton(new QPushButton("Logout",this)){

	auto groupLayout = new QHBoxLayout();
	QList<QPushButton*> buttons = { playButton,upgradesButton,optionsButton,logoutButton };
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
		emit selectedScreen(MainScreen::PlayScreen);

	}else if (clickedButton==upgradesButton){
		emit selectedScreen(MainScreen::UpgradesScreen);
	}else if (clickedButton==optionsButton){
		emit selectedScreen(MainScreen::OptionsScreen);
	}else if (clickedButton==logoutButton){
		emit selectedScreen(MainScreen::LoginScreen);
	}
}

