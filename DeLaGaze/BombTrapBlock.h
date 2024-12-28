#pragma once
#include "BreakableBlock.h"
class Map;

class BombTrapBlock: public BreakableBlock{
public:
	BombTrapBlock(std::pair<size_t, size_t> pos, Map* map);
	ObjectType GetType() const override { return ObjectType::BombTrapBlock; };
	void Print() const override;
	void OnBreak() override;
	bool HasExploded();
	virtual crow::json::wvalue toJson() override;
private:
	// Methods
	void Boom();

	// Atributes
	Map* m_map;
	float m_boomRadius = 2.5;
	bool m_hasExploded;

};

