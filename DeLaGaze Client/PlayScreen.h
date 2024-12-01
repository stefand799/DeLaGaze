#pragma once

#include <QWidget>
#include "qpushbutton.h"

class PlayScreen  : public QWidget
{
	Q_OBJECT
public:
	PlayScreen(QWidget *parent=nullptr);
	~PlayScreen();
private:
	QPushButton* ffaGameButton;
	QPushButton* teamGameButton;
};
