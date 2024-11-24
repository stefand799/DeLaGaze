#pragma once
#include <crow.h>
#include "DatabaseManager.h"
class Routes
{
	public:
		Routes(crow::SimpleApp& app);

	private:
		database::PlayerStorage player_storage;

};

