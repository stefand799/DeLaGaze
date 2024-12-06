#pragma once

#include <QWidget>
#include "qlabel.h"
#include "qpushbutton.h"

class UpgradesScreen  : public QWidget
{
	Q_OBJECT

public:
	UpgradesScreen(QWidget *parent);
	~UpgradesScreen();
private:
	QLabel* fireRateLabel;
	QLabel* bulletSpeedLabel;
	QPushButton* fireRateBuyButton;
	QPushButton* bulletSpeedBuyButton;
};
