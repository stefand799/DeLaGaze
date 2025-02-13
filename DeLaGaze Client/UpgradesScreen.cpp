#include "UpgradesScreen.h"

#include "qformlayout.h"
#include <cpr/cpr.h>
#include <crow/json.h>

UpgradesScreen::UpgradesScreen(QWidget* parent)
	:MiauScreen(":/DeLaGazeClient/images/miau_main.png", parent),
	fireRateLabel(new QLabel("Fire Rate", this)),
	bulletSpeedLabel(new QLabel("Bullet Speed", this)),
	fireRateBuyButton(new QPushButton("Buy\n(500 points)", this)),
	bulletSpeedBuyButton(new QPushButton("Buy\n(10 score)", this)),
	backButton(new QPushButton("Back", this)),
	pointsLabel(new QLabel("Points: "+QString::number(points), this)),
	scoreLabel(new QLabel("Score: "+QString::number(score), this))
{
	

	fireRateLabel->setStyleSheet("QLabel { background-color: red; }");
	bulletSpeedLabel->setStyleSheet("QLabel { background-color: yellow; }");

	auto groupLayout = new QFormLayout;
	groupLayout->setAlignment(Qt::AlignCenter);

	QList<QPair<QLabel*, QPushButton*>> items = { {fireRateLabel,fireRateBuyButton},{bulletSpeedLabel,bulletSpeedBuyButton} };
	QFont buttonFont;
	QFont labelFont;
	buttonFont.setPointSize(20);
	labelFont.setPointSize(30);
	for (auto pair:items){
		QLabel* label = pair.first;
		QPushButton* button = pair.second;
		label->setAlignment(Qt::AlignCenter);
		label->setFixedSize(400, 100);
		label->setFont(labelFont);

		button->setFixedSize(300, 100);
		button->setFont(buttonFont);

		connect(button, &QPushButton::clicked, this, &UpgradesScreen::buttonClicked);
		groupLayout->addRow(label, button);

	}

	auto groupWidget = new QWidget(this);
	groupWidget->setLayout(groupLayout);

	QFont backButtonFont;
	backButtonFont.setPointSize(15);
	backButton->setFixedSize(200, 50);
	backButton->setFont(backButtonFont);
	connect(backButton, &QPushButton::clicked, this, &UpgradesScreen::buttonClicked);


	auto topLayout = new QHBoxLayout;
	topLayout->addWidget(backButton);
	topLayout->addStretch();
	topLayout->addWidget(pointsLabel);
	topLayout->addStretch();
	topLayout->addWidget(scoreLabel);
	topLayout->addStretch();

	auto mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addStretch();

	auto centerLayout = new QHBoxLayout;
	centerLayout->addStretch();
	centerLayout->addWidget(groupWidget);
	centerLayout->addStretch();

	mainLayout->addLayout(centerLayout);
	mainLayout->addStretch();


	setLayout(mainLayout);

}

UpgradesScreen::~UpgradesScreen()
{}

void UpgradesScreen::setPointsAndLevel(int points, int fireRateLevel)
{	points = points;
	fireRateLevel = fireRateLevel;
	pointsLabel->setText("Points: " + QString::number(points));
	fireRateLabel->setText("Fire Rate\n" + QString::number(fireRateLevel) + "/4");
	update();
}

void UpgradesScreen::setScoreAndLevel(int score, bool bulletSpeedLevel)
{	score = score;
	bulletSpeedLevel = bulletSpeedLevel;
	scoreLabel->setText("Score: " + QString::number(score));
	bulletSpeedLabel->setText("Bullet Speed\n" + QString::number(bulletSpeedLevel) + "/1");
	update();
}

void UpgradesScreen::buttonClicked() {
	QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
	if (clickedButton == nullptr) {
		qWarning() << "Sender does not belong to the known buttons!";
	}
	else if (clickedButton==backButton)
	{
		emit backButtonClicked();
	}
	else if (clickedButton==bulletSpeedBuyButton)
	{
		emit bulletSpeedUpgradeRequest();
	}
	else if (clickedButton==fireRateBuyButton)
	{
		emit fireRateUpgradeRequest();
	}
}
