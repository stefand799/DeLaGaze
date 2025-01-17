#include "GameScreen.h"

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


void GameScreen::paintEvent(QPaintEvent* event)
{
    //MiauScreen::paintEvent(event);
	QPainter painter(this);
	//TODO: maybe remove for performance?
	painter.setRenderHint(QPainter::Antialiasing);

	if (!currentState || !currentState.has("map"))
        return;

	QPoint offset = calculateOffset();
	painter.translate(offset);

    auto& map = currentState["map"];
    if (map.has("map")) {
        auto& blocks = map["map"];
        for (const auto& block : blocks) {
            if (block.has("type") && block.has("x") && block.has("y")) {
                int x = block["x"].i() * GRID_SIZE;
                int y = block["y"].i() * GRID_SIZE;

                // Set different colors based on block type
                if (block["type"].s() == "UnbreakableBlock") {
                    painter.setBrush(QColor(80, 80, 80));  // Dark gray
                    painter.setPen(Qt::black);
                }
                if (block["type"].s() == "DeadlyBlock") {
                    painter.setBrush(Qt::magenta);  // Magenta
                    painter.setPen(Qt::black);
                }
                else if (block["type"].s() == "BreakableBlock") {
                    painter.setBrush(QColor(150, 75, 0));  // Brown
                    painter.setPen(Qt::black);
                }
                else if (block["type"].s() == "BombTrapBlock") {
                    painter.setBrush(Qt::red);  // Red for danger
                    painter.setPen(Qt::black);
                }
                else if (block["type"].s() == "Pathway") {
                    painter.setBrush(QColor(200, 200, 200));  // Light gray
                    painter.setPen(Qt::lightGray);
                }

                painter.drawRect(x, y, GRID_SIZE, GRID_SIZE);
            }
        }
    }


    
    // Draw players
    if (currentState.has("players")) {
        auto& players = currentState["players"];
        for (const auto& player : players) {
            if (player.has("x") && player.has("y") && player.has("username") && player.has("facing")) {
                double x = player["x"].d() * GRID_SIZE;
                double y = player["y"].d() * GRID_SIZE;

                // Draw player body
                painter.setBrush(QColor(30, 144, 255));  // Dodger Blue
                painter.setPen(Qt::black);
                painter.drawEllipse(QPointF(x, y), GRID_SIZE / 2.5, GRID_SIZE / 2.5);

                // Draw direction indicator
                painter.setBrush(Qt::black);
                double indicatorSize = GRID_SIZE / 4;
                QPointF center(x, y);
                QPointF indicator;

                QString facing = QString::fromStdString(player["facing"].s());
                if (facing == "North") {
                    indicator = center - QPointF(0, indicatorSize);
                }
                else if (facing == "South") {
                    indicator = center + QPointF(0, indicatorSize);
                }
                else if (facing == "East") {
                    indicator = center + QPointF(indicatorSize, 0);
                }
                else if (facing == "West") {
                    indicator = center - QPointF(indicatorSize, 0);
                }

                painter.drawEllipse(indicator, GRID_SIZE / 8, GRID_SIZE / 8);

                // Draw player name
                painter.setPen(Qt::black);
                QFont font = painter.font();
                font.setBold(true);
                painter.setFont(font);
                QRectF textRect(x - GRID_SIZE, y - GRID_SIZE, GRID_SIZE * 2, GRID_SIZE / 2);
                painter.drawText(textRect, Qt::AlignCenter, QString::fromStdString(player["username"].s()));
            }
        }
    }

    // Draw bullets
    if (currentState.has("bullets")) {
        auto& bullets = currentState["bullets"];
        painter.setBrush(Qt::yellow);
        painter.setPen(Qt::red);
        for (const auto& bullet : bullets) {
            if (bullet.has("x") && bullet.has("y")) {
                double x = bullet["x"].d() * GRID_SIZE;
                double y = bullet["y"].d() * GRID_SIZE;
                painter.drawEllipse(QPointF(x, y), GRID_SIZE / 6, GRID_SIZE / 6);
            }
        }
    }
}

void GameScreen::keyPressEvent(QKeyEvent* event)
{
	if (!event->isAutoRepeat())
	{
        pressedKeys.insert(event->key());
	}
    //QWidget::keyPressEvent(event);
}

void GameScreen::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
        pressedKeys.remove(event->key());
    //QWidget::keyReleaseEvent(event);
}

//void GameScreen::processInput()
//{
//    bool moved = false;
//	if (pressedKeys.contains(Qt::Key_W))
//	{
//        emit playerMoveRequest("NORTH");
//	}
//    if (pressedKeys.contains(Qt::Key_A))
//    {
//        emit playerMoveRequest("WEST");
//
//    }
//	if (pressedKeys.contains(Qt::Key_S))
//    {
//        emit playerMoveRequest("SOUTH");
//
//    }
//    if (pressedKeys.contains(Qt::Key_D))
//    {
//        emit playerMoveRequest("EAST");
//
//    }
//
//    if (pressedKeys.contains(Qt::Key_Up))
//    {
//	    
//    }
//    if (pressedKeys.contains(Qt::Key_Left))
//    {
//
//    }
//    if (pressedKeys.contains(Qt::Key_Down))
//    {
//
//    }
//    if (pressedKeys.contains(Qt::Key_Right))
//    {
//
//    }
//}

void GameScreen::processInput()
{
    bool moved = false;
    std::string direction;

	if (pressedKeys.contains(Qt::Key_W))
	{
        direction = "NORTH";
        moved = true;
	}
    else if (pressedKeys.contains(Qt::Key_A))
    {
        direction = "WEST";
        moved = true;

    }
	else if (pressedKeys.contains(Qt::Key_S))
    {
        direction = "SOUTH";
        moved = true;

    }
    else if (pressedKeys.contains(Qt::Key_D))
    {
        direction = "EAST";
        moved = true; 

    }

    else if (pressedKeys.contains(Qt::Key_Up))
    {
	    
    }
    else if (pressedKeys.contains(Qt::Key_Left))
    {

    }
    else if (pressedKeys.contains(Qt::Key_Down))
    {

    }
    else if (pressedKeys.contains(Qt::Key_Right))
    {

    }
    if (moved)
    {
        emit playerMoveRequest(direction);
        update();
    }
}

void GameScreen::onGameStateReceived(const std::string& gameStateJson)
{
	currentState = crow::json::load(gameStateJson);
	update();
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
}
