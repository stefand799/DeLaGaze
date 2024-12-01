#include "PlayScreen.h"

PlayScreen::PlayScreen(QWidget *parent)
	: QWidget(parent),
	ffaGameButton(new QPushButton("Free For All", this)),
	teamGameButton(new QPushButton("Team", this))

{}

PlayScreen::~PlayScreen()
{}
