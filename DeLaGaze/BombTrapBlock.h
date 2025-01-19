#pragma once
#include "BreakableBlock.h"
class Map;

class BombTrapBlock: public BreakableBlock{
public:
	BombTrapBlock(std::pair<size_t, size_t> pos, Map* map);
	ObjectType GetType() const override { return ObjectType::BombTrapBlock; };
	void OnBreak() override;
	bool HasExploded();
	virtual crow::json::wvalue toJson() override;
private:
	void Boom();

	Map* m_map;
	float m_boomRadius = 2.5;
	bool m_hasExploded;

};

