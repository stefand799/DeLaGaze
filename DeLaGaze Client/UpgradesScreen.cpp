#include "UpgradesScreen.h"

#include "qformlayout.h"
#include <cpr/cpr.h>
#include <crow/json.h>

UpgradesScreen::UpgradesScreen(QWidget* parent)
	:MiauScreen(":/DeLaGazeClient/images/miau_main.png", parent),
	fireRateLabel(new QLabel("Fire Rate", this)),
	bulletSpeedLabel(new QLabel("Bullet Speed", this)),
	fireRateBuyButton(new QPushButton("Buy\n(X points)", this)),
	bulletSpeedBuyButton(new QPushButton("Buy\n(Y score)", this)),
	backButton(new QPushButton("Back", this))
{
	int pointsPlaceholder = 3000;
	int scorePlaceholder = 900;
	QLabel* pointsLabel = new QLabel(this);

	fireRateLabel->setStyleSheet("QLabel { background-color: red; }");
	bulletSpeedLabel->setStyleSheet("QLabel { background-color: yellow; }");

	auto groupLayout = new QFormLayout;
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
	auto mainLayout = new QVBoxLayout;
	mainLayout->addLayout(groupLayout);
	mainLayout->setAlignment(Qt::AlignCenter);
	setLayout(mainLayout);

	QFont backButtonFont;
	backButtonFont.setPointSize(15);
	backButton->setFixedSize(200, 50);
	backButton->setFont(backButtonFont);
	connect(backButton, &QPushButton::clicked, this, &UpgradesScreen::buttonClicked);

	auto topLayout = new QHBoxLayout;
	topLayout->addWidget(backButton);
	topLayout->addStretch();
}

UpgradesScreen::~UpgradesScreen()
{}

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
