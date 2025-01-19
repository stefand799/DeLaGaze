#include "LobbyScreen.h"

#include <QHBoxLayout>


LobbyScreen::LobbyScreen(QWidget* parent)
	: MiauScreen(":/DeLaGazeClient/images/miau_main.png", parent),
	waitingLabel(new QLabel("Waiting for the game to commence . . .", this))
{
	waitingLabel->setStyleSheet("QLabel { background-color: aquamarine; }");
	QFont labelFont;
	labelFont.setPointSize(30);
	waitingLabel->setAlignment(Qt::AlignCenter);
	waitingLabel->setFixedSize(800, 100);
	waitingLabel->setFont(labelFont);

	auto mainLayout = new QVBoxLayout;
	mainLayout->addWidget(waitingLabel);
	mainLayout->setAlignment(Qt::AlignCenter);
	setLayout(mainLayout);


	hasGameStartedTimer = new QTimer(this);
	connect(hasGameStartedTimer, &QTimer::timeout, this, [&]()
		{
			qDebug() << "CHECKING WHETHER GAME HAS STARTED\n";
			emit hasGameStartedRequest();
		});
	hasGameStartedTimer->start(1000);
}

LobbyScreen::~LobbyScreen()
{}

void LobbyScreen::onGameStart()
{
	hasGameStartedTimer->stop();
}


