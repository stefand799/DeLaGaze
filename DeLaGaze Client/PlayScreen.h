#pragma once

#include <QWidget>
#include "qpushbutton.h"
#include "qtextedit.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;
class PlayScreen  : public QWidget
{
	Q_OBJECT
public:
	PlayScreen(QWidget *parent=nullptr);
	~PlayScreen();
	enum class Screen
	{
		FFAGameScreen,
		TeamGameScreen,
		MainScreen
	};
	std::vector<std::vector<std::string>> deserializeMapToGrid(const std::string& jsonString);
signals:
	void selectedScreen(PlayScreen::Screen);
private slots:
	void buttonClicked();
private:
	QPushButton* ffaGameButton;
	QPushButton* teamGameButton;
	QPushButton* backButton;
	QTextEdit* ffaGameTextEdit;
	QTextEdit* teamGameTextEdit;
	QString ffaGameDescription;
	QString teamGameDescription;
};
