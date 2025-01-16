#include "LobbyScreen.h"

#include <QHBoxLayout>


LobbyScreen::LobbyScreen(QWidget* parent)
	: MiauScreen(":/DeLaGazeClient/images/miau_main.png", parent),
	waitingLabel(new QLabel("Waiting for the game to commence . . .", this)),
	backButton(new QPushButton("Back", this))
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

	QFont backButtonFont;
	backButtonFont.setPointSize(15);
	backButton->setFixedSize(200, 50);
	backButton->setFont(backButtonFont);
	connect(backButton, &QPushButton::clicked, this, [&]() { emit backButtonClicked(); });

	auto topLayout = new QHBoxLayout;
	topLayout->addWidget(backButton);
	topLayout->addStretch();


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


