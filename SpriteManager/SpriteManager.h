#pragma once

#ifdef SPRITEMANAGER_EXPORTS
#define SPRITEMANAGER_API __declspec(dllexport)
#else
#define SPRITEMANAGER_API __declspec(dllimport)
#endif

#include <QVector>
#include <QPair>
#include <QVariant>
#include <QMovie>
#include <QPixmap>
#include <string>
#include <crow/json.h>

class SPRITEMANAGER_API SpriteManager {
public:
    SpriteManager();
    ~SpriteManager();

    QVector<QPair<QVariant, QPair<double, double>>> processGameState(const crow::json::rvalue& gameStateJson);

private:
    void processMapSprites(const crow::json::rvalue& mapData, QVector<QPair<QVariant, QPair<double, double>>>& sprites);
    void processBulletSprites(const crow::json::rvalue& bulletsData, QVector<QPair<QVariant, QPair<double, double>>>& sprites);
    void processPlayerSprites(const crow::json::rvalue& playersData, QVector<QPair<QVariant, QPair<double, double>>>& sprites);

    std::string getSpriteForMapObject(const std::string& type);
    std::string getSpriteForBullet(const std::string& direction);
    std::string getSpriteForPlayer(const std::string& state, const std::string& facing);
};
