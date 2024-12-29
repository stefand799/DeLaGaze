#include "UpgradesScreen.h"

#include "qformlayout.h"
#include <cpr/cpr.h>
#include <crow/json.h>

UpgradesScreen::UpgradesScreen(QWidget* parent)
	: QWidget(parent),
	fireRateLabel(new QLabel("Fire Rate", this)),
	bulletSpeedLabel(new QLabel("Bullet Speed", this)),
	fireRateBuyButton(new QPushButton("Buy\n(X points)", this)),
	bulletSpeedBuyButton(new QPushButton("Buy\n(Y score)", this))
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

		connect(button, &QPushButton::clicked, this, &UpgradesScreen::buyButtonClicked);
		groupLayout->addRow(label, button);

	}
	auto mainLayout = new QVBoxLayout;
	mainLayout->addLayout(groupLayout);
	mainLayout->setAlignment(Qt::AlignCenter);
	setLayout(mainLayout);
}

UpgradesScreen::~UpgradesScreen()
{}

void UpgradesScreen::buyButtonClicked() {
	/*TODO: Server requests!!!!!*/
	QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
	if (clickedButton == nullptr) {
		qWarning() << "Sender does not belong to the known buttons!";
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
