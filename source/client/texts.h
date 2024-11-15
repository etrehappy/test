#pragma once

namespace text_v
{
	static const char* kNewStr{"\n"};
	static const char* kSpace{" "};
	static const char* kBracket{"> "};
	static const char* kColon{": "};
	static const char* kDot{". "};
	static const char* kLoginReq{"Введите логин: "};
	static const char* kPasReq{"Введите пароль: "};
	static const char* kErrorInput{"Некорректный ввод. Попробуйте ещё раз."};
	static const char* kErrorLogPas{"Ошибка обработки учетных данных."};
	static const char* kErrorendMessageToServer{"Не удалось отправить информацию на сервер."};

	static const char* kAuthSuccess{"Успешная авторизация."};
	static const char* kAuthFailure{"Неуспешная авторизация. Попробуйте ещё раз."};

	static const char* kAvatarNumber{"Номер аватара: "};
	static const char* kErrorChoiseAvatar{"Не удалось выбрать аватара. Попробуйте ещё раз."};

	static const char* kErrorStartGame{"Не удалось начать игру. Попробуйте ещё раз."};
	static const char* kStartGame{"\t ******* Игра началась ******* "};
	static const char* kAvatarState{"Состояние аватара: "};
	static const char* kLvl{"ур. — "};
	static const char* kHealth{"здоровье — "};
	static const char* kMana{"мана — "};
	static const char* kCurrentLocation{"Текущая локация: "};

	static const char* kMenuNumber{"Пункт меню: "};

	static const char* kOutOfBounds{"Предупреждение: выход за границы карты."};
	static const char* kDoorClosed{"Дверь закрыта"};
	static const char* kWallInFront{"Перед тобой стена"};
	static const char* kObstacle{"Движение заблокировано: препятствие на пути"};

}/* text_v */

namespace text_p
{
	static const char* kOptDescription{"Игровые команды"};
	static const char* kCommandKey{"command"};
	static const char* kCommandKeyDescription{"Выполняемая команда (место, осмотреться, инвентарь, идти, помощь)"};
	static const char* kArgsKey{"args"};
	static const char* kArgsKeyDescription{"Аргументы команды"};

} /* text_p */

namespace command_str
{
	static const char* kMenu{"меню"};
	static const char* kSpeak{"говорить"};
	static const char* kChat{"чат"};
	static const char* kToEveryone{"всем"};
	static const uint16_t kChatMinArgsSize{2};
	static const char* kMap{"карта"};
	static const char* kLocation{"место"};
	static const char* kGo{"идти"};
	static const uint16_t kGoMinArgsSize{2};

	static const char* kNorth{"север"};
	static const char* kN{"с"};
	static const char* kSouth{"юг"};
	static const char* kS{"ю"};
	static const char* kWest{"запад"};
	static const char* kW{"з"};
	static const char* kEast{"восток"};
	static const char* kE{"в"};



}/* Commands */

namespace text_main_menu
{
	static const char* kSaveAndExit{"Выход с сохранением"};

}/* text_main_menu */