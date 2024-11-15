#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <sqlite3.h>
#include <memory>

#include "logger.h"
#include "settings.h"
#include "game_elements.h"

class Database
{
public:
	Database();
	~Database();

	void Create() const;
	std::shared_ptr<SQLite::Database> GetToRead() const;
	std::shared_ptr<SQLite::Database> GetToReadAndWrite() const;	

private:	
	void CreateTables(SQLite::Database&) const;

	/*!
	* @brief For MVP-version only
	*/
	void SetTestData(SQLite::Database&) const;

	//void OpenToRead();
	void OpenToReadAndWrite();


	//std::shared_ptr<SQLite::Database> db_read_{};
	std::shared_ptr<SQLite::Database> db_read_write_{};
};

