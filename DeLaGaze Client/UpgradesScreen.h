#pragma once

#include <QWidget>

#include "MiauScreen.h"
#include "qlabel.h"
#include "qpushbutton.h"
class UpgradesScreen  : public MiauScreen
{
	Q_OBJECT

public:
	UpgradesScreen(QWidget *parent);
	~UpgradesScreen();

	void setPointsAndLevel(int points, int fireRateLevel);
	void setScoreAndLevel(int score, bool bulletSpeedLevel);
signals:
	void bulletSpeedUpgradeRequest();
	void fireRateUpgradeRequest();
	void backButtonClicked();
private slots:
	void buttonClicked();
private:
	QLabel* fireRateLabel;
	QLabel* bulletSpeedLabel;
	QPushButton* fireRateBuyButton;
	QPushButton* bulletSpeedBuyButton;
	QPushButton* backButton;
	QLabel* pointsLabel;
	QLabel* scoreLabel;
	int points;
	int score;
	int fireRateLevel;
	int bulletSpeedLevel;
};
