#include "pch.h" // Include only if using precompiled headers


#include "SpriteManager.h"


SpriteManager::SpriteManager() {}

SpriteManager::~SpriteManager() {}

QVector<QPair<QVariant, QPair<double, double>>> SpriteManager::processGameState(const crow::json::rvalue& gameStateJson) {
    QVector<QPair<QVariant, QPair<double, double>>> sprites;

    if (gameStateJson.has("map")) {
        const auto& mapData = gameStateJson["map"]["map"];
        processMapSprites(mapData, sprites);
    }

    if (gameStateJson.has("bullets")) {
        const auto& bulletsData = gameStateJson["bullets"];
        processBulletSprites(bulletsData, sprites);
    }

    if (gameStateJson.has("players")) {
        const auto& playersData = gameStateJson["players"];
        processPlayerSprites(playersData, sprites);
    }

    return sprites;
}
//TODO: DYNAMIC FOR ALL
void SpriteManager::processMapSprites(const crow::json::rvalue& mapData, QVector<QPair<QVariant, QPair<double, double>>>& sprites) {
    for (const auto& mapObject : mapData) {
        if (mapObject.has("x") && mapObject.has("y") && mapObject.has("type")) {
            double x = mapObject["x"].d();
            double y = mapObject["y"].d();
            std::string spritePath = getSpriteForMapObject(mapObject["type"].s());
            QPixmap pixmap(QString::fromStdString(spritePath));
            if (!pixmap.isNull()) {
                sprites.append(qMakePair(QVariant(pixmap), qMakePair(static_cast<double>(x), static_cast<double>(y))));
            }
        }
    }
}

void SpriteManager::processBulletSprites(const crow::json::rvalue& bulletsData, QVector<QPair<QVariant, QPair<double, double>>>& sprites) {
    for (const auto& bullet : bulletsData) {
        double x = bullet["x"].d();
        double y = bullet["y"].d();
        std::string spritePath = getSpriteForBullet(bullet["direction"].s());
        if (spritePath.ends_with(".gif")) {
            QMovie* movie = new QMovie(QString::fromStdString(spritePath));
            if (movie->isValid()) {
                movie->start();
                sprites.append(qMakePair(QVariant::fromValue(movie), qMakePair(static_cast<double>(x), static_cast<double>(y))));
            }
            else {
                delete movie;
            }
        }
    }
}

void SpriteManager::processPlayerSprites(const crow::json::rvalue& playersData, QVector<QPair<QVariant, QPair<double, double>>>& sprites) {
    for (const auto& player : playersData) {
        double x = player["x"].d();
        double y = player["y"].d();
        std::string spritePath = getSpriteForPlayer(player["teamId"].i(),player["state"].s(), player["facing"].s());
        QMovie* movie = new QMovie(QString::fromStdString(spritePath));
        if (movie->isValid()) {
            movie->start();
            sprites.append(qMakePair(QVariant::fromValue(movie), qMakePair(static_cast<double>(x), static_cast<double>(y))));
        }
        else {
            delete movie;
        }
    }
}

std::string SpriteManager::getSpriteForMapObject(const std::string& type) {
    if (type == "Pathway") return ":/DeLaGazeClient/sprites/pathway.png";
    if (type == "UnbreakableBlock") return ":/DeLaGazeClient/sprites/unbreakable_block.png";
    if (type == "DeadlyBlock") return ":/DeLaGazeClient/sprites/deadly_block.png";
    if (type == "BreakableBlock") return ":/DeLaGazeClient/sprites/breakable_block.png";
    if (type == "BombTrapBlock") return ":/DeLaGazeClient/sprites/breakable_block.png";
    return ":/sprites/default.png";
}

std::string SpriteManager::getSpriteForBullet(const std::string& direction) {
    if (direction == "North") return ":/DeLaGazeClient/sprites/bullet_up.gif";
    if (direction == "South") return ":/DeLaGazeClient/sprites/bullet_down.gif";
    if (direction == "West") return ":/DeLaGazeClient/sprites/bullet_left.gif";
    if (direction == "East") return ":/DeLaGazeClient/sprites/bullet_right.gif";
    return ":/sprites/bullet_default.png";
}

std::string SpriteManager::getSpriteForPlayer(int teamId,const std::string& state, const std::string& facing) {
    return ":/DeLaGazeClient/sprites/player_"+std::to_string(teamId) + "_" + state + "_" + facing + ".gif";
}
