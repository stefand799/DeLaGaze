#pragma once

#include <QWidget>
#include "qlabel.h"
#include "qpushbutton.h"
/*TODO: server requests, to get the upgrade number, the player's score and points,*/
class UpgradesScreen  : public QWidget
{
	Q_OBJECT

public:
	UpgradesScreen(QWidget *parent);
	~UpgradesScreen();

signals:
	void bulletSpeedUpgradeRequest();
	void fireRateUpgradeRequest();
private slots:
	void buyButtonClicked();
private:
	QLabel* fireRateLabel;
	QLabel* bulletSpeedLabel;
	QPushButton* fireRateBuyButton;
	QPushButton* bulletSpeedBuyButton;
};
