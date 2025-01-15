#pragma once

#include <QWidget>

#include "MiauScreen.h"
#include "qpushbutton.h"
#include "qtextedit.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;
class PlayScreen  : public MiauScreen
{
	Q_OBJECT
public:
	PlayScreen(QWidget *parent=nullptr);
	~PlayScreen();
	std::vector<std::vector<std::string>> deserializeMapToGrid(const std::string& jsonString);
signals:
	void backButtonClicked();
	void joinLobbyRequest(const std::string& gameMode);
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
