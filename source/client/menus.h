#pragma once

#include <array>
#include <map>


enum class EMainMenu : uint16_t
{
    kSaveAndExit = 1

    , kEnd
};

template<typename T>
class Menu
{
public:
    Menu() = default;
    virtual ~Menu() = default;

    const std::map<T, const char*>& GetMap() { return map_; }

protected:
    std::map<EMainMenu, const char*> map_{};
};

class MainMenu : public Menu<EMainMenu>
{
public:
    MainMenu();
    ~MainMenu() = default;
};

class IExit 
{
public:    
    virtual ~IExit() = default;
    virtual void StopGame() = 0;
};
