#include "UpgradesScreen.h"

#include <qformlayout.h>

UpgradesScreen::UpgradesScreen(QWidget* parent)
	: QWidget(parent),
	fireRateLabel(new QLabel("Fire Rate", this)),
	bulletSpeedLabel(new QLabel("Bullet Speed", this)),
	fireRateBuyButton(new QPushButton("Buy", this)),
	bulletSpeedBuyButton(new QPushButton("Buy", this))
{
	auto layout = new QFormLayout;
	layout->addRow(fireRateLabel,bulletSpeedLabel);
	setLayout(layout);
}

UpgradesScreen::~UpgradesScreen()
{}
