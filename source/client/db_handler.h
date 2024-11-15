#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>
#include <mutex>

#include "game_elements.h"

class DatabaseHandler
{
public:
	DatabaseHandler();
	~DatabaseHandler();

	std::shared_ptr<Location> SetLocation(const uint32_t, std::mutex&);

private:
	void OpenDb();
	std::map<NpcName, Npc> GetNpcListData(const uint32_t);

	std::shared_ptr <SQLite::Database> db_read_;

};

