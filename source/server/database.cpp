#include "database.h"

Database::Database()
{
    Create();
    //OpenToRead();
    OpenToReadAndWrite();
}

Database::~Database()
{
}

void Database::Create() const
{
    if (!std::filesystem::exists(settings::kFilesDir)) {
        if (!std::filesystem::create_directories(settings::kFilesDir)) {
            std::cerr << "Ошибка при создании директории: " << settings::kFilesDir << std::endl; //!!!
            return;
        }
    }

    if (std::filesystem::exists(settings::server::kDatabaseFile))
    {
        /*  The MVP-version hasn't difficult rules to check database's tables. 
        */
        return;
    }

try
{
    SQLite::Database db{settings::server::kDatabaseFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE};

    db.exec("PRAGMA synchronous = NORMAL;");
    db.exec("PRAGMA journal_mode = WAL;");
    db.exec("PRAGMA cache_size = 10000;");
    db.exec("PRAGMA temp_store = MEMORY;");

    CreateTables(db);
    SetTestData(db);
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}  
}

//std::shared_ptr<SQLite::Database> Database::GetToRead() const
//{
//    return db_read_;
//}

std::shared_ptr<SQLite::Database> Database::GetToReadAndWrite() const
{
    return db_read_write_;
}

void Database::CreateTables(SQLite::Database& db) const
{
    /* user */
    db.exec("CREATE TABLE IF NOT EXISTS user"
        "(uuid          TEXT PRIMARY KEY"
        ",login         TEXT NOT NULL"
        ",password      TEXT NOT NULL"
        ",created_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")");
    
    /* avatar */
    db.exec("CREATE TABLE IF NOT EXISTS avatar"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT" /* unique avatar identifier */
        ",user_uuid TEXT NOT NULL"
        ",name TEXT NOT NULL"
        ",race_id INTEGER NOT NULL" /* e.g. human, elf */
        ",class_id INTEGER NOT NULL" /* e.g. warrior, mage */
        ",level INTEGER DEFAULT 1"
        ",experience INTEGER DEFAULT 0"
        ",health INTEGER DEFAULT 100"
        ",mana INTEGER DEFAULT 100"
        ",strength INTEGER DEFAULT 10"
        ",dexterity INTEGER DEFAULT 10"
        ",intelligence INTEGER DEFAULT 10"
        ",location_id INTEGER NOT NULL" /* avatar's current location */
        ",pos_x INTEGER NOT NULL"
        ",pos_y INTEGER NOT NULL"
        ",created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ",FOREIGN KEY(user_uuid) REFERENCES user(uuid)"
        ",FOREIGN KEY(location_id) REFERENCES location(id)"
        ",FOREIGN KEY(race_id) REFERENCES race(id)"
        ",FOREIGN KEY(class_id) REFERENCES class(id)"
        ")");

    /* location */
    db.exec("CREATE TABLE IF NOT EXISTS location"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT"
        ",name TEXT NOT NULL"
        ",description TEXT"
        ",x INTEGER"
        ",y INTEGER"
        ",z INTEGER"
        ",is_safe_zone BOOLEAN DEFAULT 0"
        ")");

    /* item */
    db.exec("CREATE TABLE IF NOT EXISTS item"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT"
        ",name TEXT NOT NULL"
        ",description TEXT"
        ",type TEXT NOT NULL" /* e.g. weapon, armour, potion */
        ",rarity TEXT NOT NULL" /* e.g. common, rare */ 
        ",price INTEGER DEFAULT 0"
        ",weight INTEGER DEFAULT 0"
        ")");

    /* inventory */
    db.exec("CREATE TABLE IF NOT EXISTS inventory"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT"
        ",avatar_id INTEGER NOT NULL"
        ",item_id INTEGER NOT NULL"
        ",quantity INTEGER DEFAULT 0"
        ",is_equipped BOOLEAN DEFAULT 0" /* if the item is worn (e.g. weapon) */
        ",FOREIGN KEY(avatar_id) REFERENCES avatar(id)"
        ",FOREIGN KEY(item_id) REFERENCES item(id)"
        ")");


    /* npc */
    db.exec("CREATE TABLE IF NOT EXISTS npc"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT"
        ",name TEXT NOT NULL"
        ",race_id INTEGER NOT NULL"
        ",description TEXT"
        ",health INTEGER DEFAULT 100"
        ",location_id INTEGER NOT NULL"
        ",pos_x INTEGER NOT NULL"
        ",pos_y INTEGER NOT NULL"
        ",FOREIGN KEY(location_id) REFERENCES location(id)"
        ",FOREIGN KEY(race_id) REFERENCES race(id)"
        ")");

    /* race */
    db.exec("CREATE TABLE IF NOT EXISTS race"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT"
        ",name TEXT NOT NULL"
        ",description TEXT"
        ");");

    /* class */
    db.exec("CREATE TABLE IF NOT EXISTS class"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT"
        ",name TEXT NOT NULL"
        ",description TEXT"
        ");");

    /* npc_dialogue */
    db.exec("CREATE TABLE npc_dialogue"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT"
        ",npc_id INTEGER NOT NULL"
        ",dialogue_type_id INTEGER NOT NULL"
        ",dialogue_text TEXT"
        ",FOREIGN KEY(npc_id) REFERENCES npc(id)"
        ",FOREIGN KEY(dialogue_type_id) REFERENCES dialogue_type(id)"
        ");");

    /* dialogue_type */
    db.exec("CREATE TABLE dialogue_type"
        "(id INTEGER PRIMARY KEY AUTOINCREMENT"
        ",type TEXT NOT NULL"
        ");");    
}


void Database::SetTestData(SQLite::Database& db) const
{
    /* test_user */
    SQLite::Statement query_test_user(db, "INSERT INTO user (uuid, login, password) VALUES (?, ?, ?)");
    query_test_user.bind(1, "1user");
    query_test_user.bind(2, "user1");
    query_test_user.bind(3, "123");
    query_test_user.exec();
    query_test_user.reset(); 
    
    query_test_user.bind(1, "2user");
    query_test_user.bind(2, "user2");
    query_test_user.bind(3, "123");
    query_test_user.exec();
    query_test_user.reset();

    /* test_location */
    SQLite::Statement query_test_location(db, "INSERT INTO location (name, x, y, is_safe_zone, description) VALUES (?, ?, ?, ?, ?)");
    query_test_location.bind(1, "Таверна");
    query_test_location.bind(2, 16);
    query_test_location.bind(3, 8);
    query_test_location.bind(4, "true");
    query_test_location.bind(5, "Это уютное место в центре города.Здесь всегда пахнет свежим хлебом и жареным мясом, а каждого вошедшого встречает добродушная Хозяйка.Массивные деревянные столы и скамьи расставлены по всему залу, а в углу у камина всегда можно найти свободное место, чтобы отдохнуть от мирских забот.В таверне можно встретить самых разных людей — от местных жителей до странствующих приключенцев.");
    query_test_location.exec();
    query_test_location.reset();

    /* test_race */
    SQLite::Statement query_test_race(db, "INSERT INTO race (name) VALUES (?)");
    //query_test_race.bind(1, "Человек");
    //query_test_race.exec();
    //query_test_race.reset();
    //query_test_race.bind(1, "Эльф");
    //query_test_race.exec();
    //query_test_race.reset();
    //query_test_race.bind(1, "Гном");
    //query_test_race.exec();
    //query_test_race.reset();

    std::vector<std::string> races = {"Человек", "Эльф", "Гном"};

    auto insert_class = [&db, &query_test_race](const std::string& race_name) 
    {        
        query_test_race.bind(1, race_name);
        query_test_race.exec();
        query_test_race.reset();
    };

    for (const auto& race_name : races) { insert_class(race_name); }


    /* test_class */
    SQLite::Statement query_test_class(db, "INSERT INTO class (name) VALUES (?)");
    query_test_class.bind(1, "Воин");
    query_test_class.exec();
    query_test_class.reset();
    query_test_class.bind(1, "Маг");
    query_test_class.exec();
    query_test_class.reset();


    /* test_avatar */
    SQLite::Statement query_test_avatar(db, "INSERT INTO avatar (user_uuid, name, race_id, class_id, location_id, pos_x, pos_y) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query_test_avatar.bind(1, "1user");
    query_test_avatar.bind(2, "human_avatar");
    query_test_avatar.bind(3, 1);
    query_test_avatar.bind(4, 1);
    query_test_avatar.bind(5, 1);
    query_test_avatar.bind(6, 13);
    query_test_avatar.bind(7, 2);
    query_test_avatar.exec();
    query_test_avatar.reset();

    query_test_avatar.bind(1, "2user");
    query_test_avatar.bind(2, "elf_avatar");
    query_test_avatar.bind(3, 2);
    query_test_avatar.bind(4, 2);
    query_test_avatar.bind(5, 1);
    query_test_avatar.bind(6, 13);
    query_test_avatar.bind(7, 2);
    query_test_avatar.exec();
    query_test_avatar.reset();

    /* test_npc */
    SQLite::Statement query_test_npc(db, "INSERT INTO npc (name, race_id, location_id, description, pos_x, pos_y) VALUES (?, ?, ?, ?, ?, ?)");
    query_test_npc.bind(1, "Хозяйка Лора");
    query_test_npc.bind(2, 3);
    query_test_npc.bind(3, 1);
    query_test_npc.bind(4, "Лора — гном, владелица таверны. Она энергична и всегда в движении, приветствует каждого посетителя с широкой улыбкой. Хозяйка знает всё и всех в округе, готова поделиться новостями и помочь советом. Ее радушие и добродушие сделали таверну любимым местом для всех, кто желает отдохнуть. Лора также умело справляется с трудностями и не стесняется устанавливать порядок, когда это необходимо.");
    query_test_npc.bind(5, 1);
    query_test_npc.bind(6, 4);
    query_test_npc.exec();
    query_test_npc.reset();

    query_test_npc.bind(1, "Помощник Адрик");
    query_test_npc.bind(2, 1);
    query_test_npc.bind(3, 1);
    query_test_npc.bind(4, "Адрик — подросток, помощник Лоры. Он всегда готов помочь с любой задачей, будь то обслуживание столов или растопка камина. Адрик мечтает стать великим воином и с удовольствием слушает рассказы гостей о приключениях и подвигах. Несмотря на юный возраст, он весьма трудолюбив и ответственен, всегда стремится угодить посетителям и учиться у Лоры мастерству управления таверной.");
    query_test_npc.bind(5, 14);
    query_test_npc.bind(6, 7);
    query_test_npc.exec();
    query_test_npc.reset();

    /* test_dialogue_type */
    SQLite::Statement query_test_dialogue_type(db, "INSERT INTO dialogue_type (type) VALUES (?)");
    query_test_dialogue_type.bind(1, "greeting");
    query_test_dialogue_type.exec();
    query_test_dialogue_type.reset();

    query_test_dialogue_type.bind(1, "task");
    query_test_dialogue_type.exec();
    query_test_dialogue_type.reset();

    query_test_dialogue_type.bind(1, "respons");
    query_test_dialogue_type.exec();
    query_test_dialogue_type.reset();

    /* test_npc_dialogue */
    SQLite::Statement query_test_npc_dialogue(db, "INSERT INTO npc_dialogue (npc_id, dialogue_type_id, dialogue_text) VALUES (?, ?, ?)");
    query_test_npc_dialogue.bind(1, 1);
    query_test_npc_dialogue.bind(2, 1);
    query_test_npc_dialogue.bind(3, "Добро пожаловать в таверну!");
    query_test_npc_dialogue.exec();
    query_test_npc_dialogue.reset();

    query_test_npc_dialogue.bind(1, 1);
    query_test_npc_dialogue.bind(2, 2);
    query_test_npc_dialogue.bind(3, "Ты можешь помочь мне собрать ингредиенты для супа?");
    query_test_npc_dialogue.exec();
    query_test_npc_dialogue.reset();

    query_test_npc_dialogue.bind(1, 1);
    query_test_npc_dialogue.bind(2, 3);
    query_test_npc_dialogue.bind(3, "Нет, я очень занят");
    query_test_npc_dialogue.exec();
    query_test_npc_dialogue.reset();

    query_test_npc_dialogue.bind(1, 1);
    query_test_npc_dialogue.bind(2, 3);
    query_test_npc_dialogue.bind(3, "Да, что нужно собрать?");
    query_test_npc_dialogue.exec();
    query_test_npc_dialogue.reset();

    query_test_npc_dialogue.bind(1, 1);
    query_test_npc_dialogue.bind(2, 2);
    query_test_npc_dialogue.bind(3, "Возьми этот ключ, чтобы открыть дверь, и принеси мне 10 томатов");
    query_test_npc_dialogue.exec();
    query_test_npc_dialogue.reset();

    query_test_npc_dialogue.bind(1, 2);
    query_test_npc_dialogue.bind(2, 1);
    query_test_npc_dialogue.bind(3, "Привет! Хочешь узнать последние новости?");
    query_test_npc_dialogue.exec();
    query_test_npc_dialogue.reset();

    /* test_item */
    SQLite::Statement query_test_item(db, "INSERT INTO item (name, type, rarity) VALUES (?, ?, ?)");
    query_test_item.bind(1, "door_key");
    query_test_item.bind(2, "other");
    query_test_item.bind(3, "common");
    query_test_item.exec();
    query_test_item.reset();    
}

//void Database::OpenToRead()
//{
//try
//{
//    db_read_ = std::make_shared<SQLite::Database>(settings::kDatabaseFile, SQLite::OPEN_READONLY | SQLite::OPEN_SHAREDCACHE);
//
//    db_read_->exec("PRAGMA synchronous = NORMAL;");
//    db_read_->exec("PRAGMA journal_mode = WAL;");
//    db_read_->exec("PRAGMA cache_size = 10000;");
//    db_read_->exec("PRAGMA temp_store = MEMORY;");
//}
//catch (const std::exception& e)
//{
//    MVP_LOG_EXEPTION(e)
//}
//}

void Database::OpenToReadAndWrite()
{
try
{
    db_read_write_ = std::make_shared<SQLite::Database>(settings::server::kDatabaseFile, SQLite::OPEN_READWRITE | SQLite::OPEN_SHAREDCACHE);

    db_read_write_->exec("PRAGMA synchronous = NORMAL;");
    db_read_write_->exec("PRAGMA journal_mode = WAL;");
    db_read_write_->exec("PRAGMA cache_size = 10000;");
    db_read_write_->exec("PRAGMA temp_store = MEMORY;");
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}