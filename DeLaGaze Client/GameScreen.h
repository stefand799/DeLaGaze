#pragma once

#include <crow/json.h>

#include "MiauScreen.h"
#include "GameClientReqManager.h"
#include "qtimer.h"

class GameScreen  : public MiauScreen
{
	Q_OBJECT

public:
	GameScreen(QWidget *parent);
	~GameScreen();
signals:
	void getGameStateRequest();
protected:
	void paintEvent(QPaintEvent* event) override;
public slots:
	void onGameStateReceived(const std::string& gameStateJson);
private:
	static constexpr int GRID_SIZE = 50;
	QTimer* gameStateTimer;
	crow::json::rvalue currentState;
	QPoint calculateOffset();
};
