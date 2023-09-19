#pragma once

#include "Chat.h"
#include "Menu.h"

#include <string>
#include <memory>

//! Консоль программы
/*!
Предоставляет интерфейс пользователя посредством текстового меню в терминале.
*/
class Console
{

public:

	Console(std::shared_ptr<Chat> chat) {
		Console::_chat = chat;
		makeOverviewMenu();
		makeChatMenu();
		makeEntranceMenu();
		makeConnectMenu();
	};

	void routine();

private:

	//Статика для доступа из передаваемых в меню функций
	static std::shared_ptr<Chat> _chat;

	static auto request(const std::string& question)->std::unique_ptr<std::string>;

    //static menu
	static std::unique_ptr<Menu> _menuEntrance;
	static std::unique_ptr<Menu> _menuConnect;
	static std::unique_ptr<Menu> _menuChat;
	static std::unique_ptr<Menu> _menuOverview;
	static void makeChatMenu();
	static void makeConnectMenu();
	static void makeEntranceMenu();
	static void makeOverviewMenu();

	//dynamic menu
	static auto makeConfigureMenu()->std::unique_ptr<Menu>;
	static auto makeRecipientMenu()->std::unique_ptr<Menu>;

    //menu functions
	static void login();
	static void registraion();
	static void overview();

	static void compose();
	static void receive();

	static void printUserStatus();
	static void printChatStatus();
	static void printConnectStatus();

	static void overviewDatabases();
    static void overviewTables();
    static void overviewUsers();
    static void overviewMesgs();
    static void overviewQuery();

    static void chat();
    static void connect();
    static void disconnect();
    static void configure();

    static void confTry();
    static void confReset();
    static void confHost();
    static void confUser();
    static void confPasswd();
    static void confDB();

};






