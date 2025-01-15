#pragma once

#include "MiauScreen.h"

class GameScreen  : public MiauScreen
{
	Q_OBJECT

public:
	GameScreen(QWidget *parent);
	~GameScreen();
};
