#pragma once

#include <qlabel.h>
#include <crow/json.h>

#include "MiauScreen.h"
#include "GameClientReqManager.h"
#include "qtimer.h"
#include "QKeyEvent"
#include "qset.h"
#include "QMovie"
#include "QGraphicsView"
#include "SpriteManager.h"

class GameScreen  : public MiauScreen
{
	Q_OBJECT

public:
	GameScreen(QWidget *parent);
	~GameScreen();
signals:
	void getGameStateRequest();
	void playerMoveRequest(const std::string& direction);
	void playerFaceRequest(const std::string& direction);
	void playerShootRequest();
protected:
	void paintEvent(QPaintEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
public slots:
	void onGameStateReceived(const std::string& gameStateJson);
private:
	void clearSprites();
	void connectMovie(QMovie* movie);
	static constexpr int GRID_SIZE = 50;

	QTimer* gameStateTimer;
	QTimer* inputTimer;

	QSet<int> pressedKeys;

	crow::json::rvalue currentState;
	QPoint calculateOffset();
	void processInput();
	SpriteManager spriteManager;
	QVector<QPair<QVariant, QPair<double, double>>> sprites;
	QVector<QMovie*> activeMovies;

};
