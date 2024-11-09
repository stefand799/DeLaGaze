#pragma once
#include "Object.h"
#include <iostream>
#include <vector>
#include <chrono>

class Map
{
public:
	Map();
	~Map() = default;

	//Getter and or Setter
	const std::vector<Object*>& operator[](size_t line) const;
	std::vector<Object*>& operator[](size_t line);

	bool generateMap(std::vector<Object*>& objects);


private:
	//Methods

//Constants
	const uint8_t kMinMapWidth = 12, kMminMapHeight = 8,
		kMaxMapWidth = 20, kMaxMapHeight = 12; // Magic numbers :)) taking as a reference 50px, 1920/50 = 38.4 and 1080/50 = 21.6

	//Atributes
	std::vector<std::vector<Object*>> m_matrix;
	std::vector<Object*> m_objects;
	uint8_t m_mapWidth, m_mapHeight;
	uint32_t m_seed; //IDEA: *using a constructor* implementation of custom seed when creating a new game


	//DEBUG METHODS:
	void __DEBUG_MAP__();
};

