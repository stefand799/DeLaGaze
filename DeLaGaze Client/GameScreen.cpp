#include "GameScreen.h"

#include <qlabel.h>

#include "GameClientReqManager.h"
#include "qpainter.h"

GameScreen::GameScreen(QWidget *parent)
	:MiauScreen(":/DeLaGazeClient/images/miau_main.png", parent),
    gameStateTimer (new QTimer(this)),
	inputTimer(new QTimer(this))
{
    setFocusPolicy(Qt::StrongFocus);

	connect(gameStateTimer, &QTimer::timeout, this, [this]()
		{
			emit getGameStateRequest();
		});
	gameStateTimer->start(16);

    connect(inputTimer, &QTimer::timeout, this, &GameScreen::processInput);
    inputTimer->start(16);
}


void GameScreen::keyPressEvent(QKeyEvent* event)
{
	if (!event->isAutoRepeat())
	{
        pressedKeys.insert(event->key());
	}
}

void GameScreen::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
        pressedKeys.remove(event->key());
}



void GameScreen::processInput()
{
    bool moved = false;
    bool faced = false;
    std::string moveDirection;
    std::string faceDirection;

	if (pressedKeys.contains(Qt::Key_W))
	{
        moveDirection = "NORTH";
        moved = true;
	}
    else if (pressedKeys.contains(Qt::Key_A))
    {
        moveDirection = "WEST";
        moved = true;

    }
	else if (pressedKeys.contains(Qt::Key_S))
    {
        moveDirection = "SOUTH";
        moved = true;

    }
    else if (pressedKeys.contains(Qt::Key_D))
    {
        moveDirection = "EAST";
        moved = true;

    }

	if (pressedKeys.contains(Qt::Key_Up))
    {
        faceDirection = "NORTH";
        faced = true;
    }
    else if (pressedKeys.contains(Qt::Key_Left))
    {
        faceDirection = "WEST";
        faced = true;
    }
    else if (pressedKeys.contains(Qt::Key_Down))
    {
        faceDirection = "SOUTH";
        faced = true;
    }
    else if (pressedKeys.contains(Qt::Key_Right))
    {
        faceDirection = "EAST";
        faced = true;
    }

    

    if (moved)
    {
        emit playerMoveRequest(moveDirection);
        update();
    }
    if (faced)
    {
        emit playerFaceRequest(faceDirection);
        update();
    }
    if (pressedKeys.contains(Qt::Key_Space))
    {
        emit playerShootRequest();
        update();
    }
}

void GameScreen::onGameStateReceived(const std::string& gameStateJson)
{
	currentState = crow::json::load(gameStateJson);
    sprites.clear();
	update();
}

void GameScreen::clearSprites()
{
    for (auto movie: activeMovies)
    {
        movie->stop();
        delete movie;
    }
    activeMovies.clear();
}

void GameScreen::connectMovie(QMovie* movie)
{
    connect(movie, &QMovie::frameChanged, this, [this]
        {
            update();
        });
}

QPoint GameScreen::calculateOffset()
{
	if (!currentState) return QPoint(0, 0);

	auto& map = currentState["map"];
	int mapWidth = map["width"].i() * GRID_SIZE;
	int mapHeight = map["height"].i() * GRID_SIZE;

	int offsetX=(width() - mapWidth) / 2;
	int offsetY = (height() - mapHeight) / 2;

	return QPoint(offsetX, offsetY);
}


GameScreen::~GameScreen()
{
	gameStateTimer->stop();
    inputTimer->stop();
    clearSprites();
    
}

void GameScreen::paintEvent(QPaintEvent* event) {
    MiauScreen::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!currentState || !currentState.has("map"))
        return;

    clearSprites();
    sprites = spriteManager.processGameState(currentState);

    for (const auto& spriteData : sprites) {
        if (spriteData.first.canConvert<QMovie*>()) {
            QMovie* movie = spriteData.first.value<QMovie*>();
            activeMovies.append(movie);
            connectMovie(movie);
            movie->start();
        }
    }

    QPointF offset = calculateOffset();
    painter.translate(offset);

    for (const auto& spriteData : sprites) {
        QVariant spriteVariant = spriteData.first;
        double gridX = spriteData.second.first*GRID_SIZE;  
        double gridY = spriteData.second.second*GRID_SIZE;

        double pixelX = (spriteData.second.first - 0.5) * GRID_SIZE;  
        double pixelY = (spriteData.second.second - 0.5) * GRID_SIZE;

        if (spriteVariant.canConvert<QPixmap>()) {
            QPixmap pixmap = spriteVariant.value<QPixmap>();
            painter.drawPixmap(gridX, gridY, pixmap);
        }
        else if (spriteVariant.canConvert<QMovie*>()) {
            QMovie* movie = spriteVariant.value<QMovie*>();
            painter.drawPixmap(pixelX, pixelY, movie->currentPixmap());
        }
    }
}

