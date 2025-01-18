#include "pch.h" // Include only if using precompiled headers


#include "SpriteManager.h"


SpriteManager::SpriteManager() {}

SpriteManager::~SpriteManager() {}

QVector<QPair<QVariant, QPair<double, double>>> SpriteManager::processGameState(const crow::json::rvalue& gameStateJson) {
    QVector<QPair<QVariant, QPair<double, double>>> sprites;

    if (gameStateJson.has("map")) {
        const auto& mapData = gameStateJson["map"];
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

void SpriteManager::processMapSprites(const crow::json::rvalue& mapData, QVector<QPair<QVariant, QPair<double, double>>>& sprites) {
    for (const auto& mapObject : mapData) {
        int x = mapObject["x"].i();
        int y = mapObject["y"].i();
        std::string spritePath = getSpriteForMapObject(mapObject["type"].s());
        QPixmap pixmap(QString::fromStdString(spritePath));
        if (!pixmap.isNull()) {
            sprites.append(qMakePair(QVariant(pixmap), qMakePair(static_cast<double>(x), static_cast<double>(y))));
        }
    }
}

void SpriteManager::processBulletSprites(const crow::json::rvalue& bulletsData, QVector<QPair<QVariant, QPair<double, double>>>& sprites) {
    for (const auto& bullet : bulletsData) {
        int x = bullet["x"].i();
        int y = bullet["y"].i();
        std::string spritePath = getSpriteForBullet(bullet["direction"].s());
        if (spritePath.ends_with(".png")) {
            QPixmap pixmap(QString::fromStdString(spritePath));
            if (!pixmap.isNull()) {
                sprites.append(qMakePair(QVariant(pixmap), qMakePair(static_cast<double>(x), static_cast<double>(y))));
            }
        }
        else if (spritePath.ends_with(".gif")) {
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
        int x = player["x"].i();
        int y = player["y"].i();
        std::string spritePath = getSpriteForPlayer(player["state"].s(), player["facing"].s());
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
    if (type == "Pathway") return ":/sprites/pathway.png";
    if (type == "UnbreakableBlock") return ":/sprites/unbreakable_block.png";
    if (type == "DeadlyBlock") return ":/sprites/deadlyblock.png";
    if (type == "BreakableBlock") return ":/sprites/breakable_block.png";
    if (type == "BombTrapBlock") return ":/sprites/breakable_block.png";
    return ":/sprites/default.png";
}

std::string SpriteManager::getSpriteForBullet(const std::string& direction) {
    if (direction == "North") return ":/sprites/bullet_up.gif";
    if (direction == "South") return ":/sprites/bullet_down.gif";
    if (direction == "West") return ":/sprites/bullet_left.gif";
    if (direction == "East") return ":/sprites/bullet_right.gif";
    return ":/sprites/bullet_default.png";
}

std::string SpriteManager::getSpriteForPlayer(const std::string& state, const std::string& facing) {
    if (state == "Idle" && facing == "North") return ":/sprites/player_idle_up.gif";
    if (state == "Idle" && facing == "South") return ":/sprites/player_idle_down.gif";
    if (state == "Idle" && facing == "West") return ":/sprites/player_idle_left.gif";
    if (state == "Idle" && facing == "East") return ":/sprites/player_idle_right.gif";

    if (state == "Shooting" && facing == "North") return ":/sprites/player_shoot_up.gif";
    if (state == "Shooting" && facing == "South") return ":/sprites/player_shoot_down.gif";
    if (state == "Shooting" && facing == "West") return ":/sprites/player_shoot_left.gif";
    if (state == "Shooting" && facing == "East") return ":/sprites/player_shoot_right.gif";

    return ":/sprites/player_default.gif";
}
