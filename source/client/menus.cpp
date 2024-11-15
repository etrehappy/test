#include "menus.h"
#include "texts.h"

using namespace text_main_menu;

MainMenu::MainMenu()   
{    
    Menu::map_[EMainMenu::kSaveAndExit] = kSaveAndExit;
}