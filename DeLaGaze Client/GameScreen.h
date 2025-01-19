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
	//void resizeEvent(QResizeEvent* event) override;
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
	QVector<QPixmap> currentFrames;
	QVector<QMovie*> activeMovies;

};
//#include <QGraphicsScene>
//#include <QGraphicsView>
//#include <QGraphicsPixmapItem>
//#include <QLabel>
//#include <QMovie>
//
//void renderSprites(QGraphicsScene* scene, QVector<QPair<QVariant, QPair<double, double>>>& sprites) {
//    for (const auto& spriteData : sprites) {
//        QVariant spriteVariant = spriteData.first;
//        double x = spriteData.second.first;
//        double y = spriteData.second.second;
//
//        if (spriteVariant.canConvert<QPixmap>()) {
//            // Static sprite
//            QPixmap pixmap = spriteVariant.value<QPixmap>();
//            QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(pixmap);
//            pixmapItem->setPos(x, y);
//            scene->addItem(pixmapItem);
//        }
//        else if (spriteVariant.canConvert<QMovie*>()) {
//            // Animated sprite
//            QMovie* movie = spriteVariant.value<QMovie*>();
//            QLabel* label = new QLabel();
//            label->setMovie(movie);
//            label->setGeometry(x, y, movie->frameRect().width(), movie->frameRect().height());
//            movie->start();
//            label->show();
//        }
//    }
//}
//
//int main(int argc, char* argv[]) {
//    QApplication app(argc, argv);
//
//    QGraphicsScene scene;
//    QGraphicsView view(&scene);
//
//    // Example usage of SpriteManager
//    SpriteManager spriteManager;
//    crow::json::wvalue gameStateJson = /* ... populate your JSON ... */;
//    QVector<QPair<QVariant, QPair<double, double>>> sprites = spriteManager.processGameState(gameStateJson);
//
//    // Render sprites on the scene
//    renderSprites(&scene, sprites);
//
//    view.show();
//
//    return app.exec();
//}