#include "Console.h"
#include "Chat.h"

#include <iostream>
#include <string>
#include <memory>

std::shared_ptr<Chat> Console::_chat;
std::unique_ptr<Menu> Console::_menuEntrance;
std::unique_ptr<Menu> Console::_menuChat;
std::unique_ptr<Menu> Console::_menuOverview;
std::unique_ptr<Menu> Console::_menuConnect;


void Console::registraion()
{
	auto login = request(std::string("Login"));

	if (_chat->usersExists(*login)) {
		std::cout << "! Login exists" << std::endl;
	}
	else {
		auto password = request(std::string("Password"));
		_chat->usersRegist(*login, *password);
	};

};


void Console::login()
{
	auto login = request(std::string("Login"));

	if (_chat->usersExists(*login)) {

		auto password = request(std::string("Password"));
		_chat->login(*login, *password);

		auto id = _chat->activeID();
		if (id) {
			_menuChat->menu();
		}
		else {
			std::cout << "! Login failed" << std::endl;
			;
		}
	}
	else {
		std::cout << "! Login is not registered" << std::endl;
	};

};


void Console::overview() {
	_menuOverview->menu();
};


void Console::compose()
{
	auto text = request(std::string("Message"));
	auto userlist = _chat->usersID();

	auto menu = makeRecipientMenu();
	menu->addOption(std::string("ALL"));


	if (userlist) {

		for (auto str : ((*userlist))) {
			Chat::ID id = std::make_shared<std::string>(str);
			auto name = _chat->userName(id);
			menu->addOption((*name) + " (ID:" + str + ")");
		};

	}
	else {
		std::cout << "#! USERLIST = NULL " << std::endl;
	};

	auto idx = (menu->menu()) - 1;
	if (idx >= 0) {
		Chat::ID idRec = std::make_shared<std::string>((*userlist)[idx]);
		_chat->send(*text, idRec);
	} else {
		_chat->send(*text);
	};

};


void Console::receive()
{
	if (_chat->unread() > 0) {
		auto msg = _chat->take();

		if (msg) {
			auto idSender = msg->sender;
			auto nameSender = _chat->userName(idSender);
			std::cout << std::endl << std::endl << "#||||||||||||||||||||#" << std::endl;
			std::cout << "<< Sender: '" << *nameSender << "' (ID" << *idSender << ")" << std::endl;
			std::cout << "<< Text: " << msg->text << std::endl;
		}
		else {
			std::cout << "! ERR receive message" << std::endl;
		};

	}
	else {
		std::cout << "! No unread msgs" << std::endl;
	};

};


void Console::routine()
{
	std::cout << std::endl << "# CONSOLE CHAT. SkillFactory project (Unit#25 - Database)" << std::endl;
	_menuConnect->menu();
};


void Console::makeEntranceMenu()
{
	if (!_menuEntrance) { _menuEntrance = std::make_unique<Menu>(); };

	_menuEntrance->setFlagZeroOptionLast(true);
	_menuEntrance->setFlagRepeatMenu(true);
	_menuEntrance->setFlagExpectUserInput(true);
	_menuEntrance->setFlagPromtUntilValid(true);

	_menuEntrance->setHeader(std::string("*** Entrance Menu ***"), printChatStatus);
	_menuEntrance->setPromt(std::string("---"));


	auto exit_option = _menuEntrance->addOption(std::string("disconnect <-"), disconnect);
	_menuEntrance->markExit(exit_option);

	_menuEntrance->addOption(std::string("login (->)"), login);
	_menuEntrance->addOption(std::string("registration"), registraion);
	_menuEntrance->addOption(std::string("overview"), overview);



	_menuEntrance->setFooter(std::string("*********************"));
};


void Console::makeConnectMenu()
{
	if (!_menuConnect) { _menuConnect = std::make_unique<Menu>(); };

	_menuConnect->setFlagZeroOptionLast(true);
	_menuConnect->setFlagRepeatMenu(true);
	_menuConnect->setFlagExpectUserInput(true);
	_menuConnect->setFlagPromtUntilValid(true);

	_menuConnect->setHeader(std::string("*** Connect Menu ***"), printConnectStatus);
	_menuConnect->setPromt(std::string("---"));


	auto exit_option = _menuConnect->addOption(std::string("EXIT"));
	_menuConnect->markExit(exit_option);

	_menuConnect->addOption(std::string("(->) chat "), chat);
	_menuConnect->addOption(std::string("connect"), connect);
	_menuConnect->addOption(std::string("disconnect"), disconnect);
	_menuConnect->addOption(std::string("(->) configure "), configure);

	_menuEntrance->setFooter(std::string("*********************"));
};


void Console::makeChatMenu()
{
	if (!_menuChat) { _menuChat = std::make_unique<Menu>(); };

	_menuChat->setFlagZeroOptionLast(true);
	_menuChat->setFlagRepeatMenu(true);
	_menuChat->setFlagExpectUserInput(true);
	_menuChat->setFlagPromtUntilValid(true);

	_menuChat->setHeader(std::string("*** Chat Menu ***"), printUserStatus);
	_menuChat->setPromt(std::string("---"));

	auto exit_option = _menuChat->addOption(std::string("(<-) logout "));
	_menuChat->markExit(exit_option);

	_menuChat->addOption(std::string("update status")); //reenter menu
	_menuChat->addOption(std::string("read message"), receive);
	_menuChat->addOption(std::string("send message"), compose);

	_menuChat->setFooter(std::string("*********************"));
};


void Console::makeOverviewMenu()
{
	if (!_menuOverview) { _menuOverview = std::make_unique<Menu>(); };

	_menuOverview->setFlagZeroOptionLast(true);
	_menuOverview->setFlagRepeatMenu(true);
	_menuOverview->setFlagExpectUserInput(true);
	_menuOverview->setFlagPromtUntilValid(true);

	_menuOverview->setHeader(std::string("*** Overview server Menu ***"), printChatStatus);
	_menuOverview->setPromt(std::string("---"));

	auto exit_option = _menuOverview->addOption(std::string("(<-) return "));
	_menuOverview->markExit(exit_option);

	_menuOverview->addOption(std::string("QUERY"), overviewQuery);
	_menuOverview->addOption(std::string("update status")); //reenter menu
	_menuOverview->addOption(std::string("databases"), overviewDatabases);
	_menuOverview->addOption(std::string("tables"), overviewTables);
	_menuOverview->addOption(std::string("users"), overviewUsers);
	_menuOverview->addOption(std::string("messages"), overviewMesgs);

	_menuOverview->setFooter(std::string("*********************"));
};


auto Console::makeConfigureMenu() -> std::unique_ptr<Menu> {

	auto menu = std::make_unique<Menu>();

	menu->setFlagZeroOptionLast(true);
	menu->setFlagRepeatMenu(false);
	menu->setFlagExpectUserInput(true);
	menu->setFlagPromtUntilValid(true);

	menu->setHeader(std::string("* Parameters:"), printConnectStatus);


	auto exit_option = menu->addOption(std::string("(<-) return "));
	menu->markExit(exit_option);

	return menu;
};


auto Console::makeRecipientMenu() -> std::unique_ptr<Menu>
{
	auto menu = std::make_unique<Menu>();

	menu->setFlagZeroOptionLast(true);
	menu->setFlagRepeatMenu(false);
	menu->setFlagExpectUserInput(true);
	menu->setFlagPromtUntilValid(false);

	menu->setHeader(std::string("* Recepients:"));

	return menu;
};


auto Console::request(const std::string& question) -> std::unique_ptr<std::string>
{
	auto answer = std::make_unique<std::string>();
	std::cout << ">> " << question << " : ";

	std::getline(std::cin, *answer);
	//std::cin >> (*answer);
	return answer;
};


void Console::printUserStatus()
{

	_chat->printStat();

	auto id = _chat->activeID();

	std::cout << "* user: ID" << *id
		<< " " << _chat->activeName()
		<< " (aka '" << *(_chat->userName(id)) << "')"
		<< " unread " << _chat->unread()
		<< std::endl;


};

void Console::printChatStatus() {
	_chat->printStat();
};


void Console::overviewDatabases() {
	_chat->print("SHOW DATABASES");
};


void Console::overviewTables() {
	_chat->print("SHOW TABLES");
};


void Console::overviewUsers() {
	_chat->print("SELECT * FROM users");
};


void Console::overviewMesgs() {
	_chat->print("SELECT * FROM mesgs");
};


void Console::overviewQuery() {
	auto query = request(std::string("Query : "));
	_chat->print(query->c_str());
};


void Console::printConnectStatus() {
    _chat->printStat();
};


void Console::chat() {

    connect();

    if (_chat->status()) {
        _menuEntrance->menu();
    } else {
        std::cout << "! Database connection is required " << std::endl;
    };
};


void Console::connect() {
    _chat->connect();
};


void Console::disconnect() {
    if ( _chat->status()) _chat->disconnect();
};


void Console::confReset() {
    _chat->reset();
};


void Console::configure() {

    std::unique_ptr<Menu> menu;

	do {

	menu = makeConfigureMenu();

	menu->addOption(std::string("TRY (connect)"), confTry);
	menu->addOption(std::string("RESET (defaults)"), confReset);

    auto conf = _chat->conf();

	menu->addOption(std::string("host = " + conf.host), confHost);
	menu->addOption(std::string("user = " + conf.user), confUser);
	menu->addOption(std::string("passwd = " + conf.passwd), confPasswd);
	menu->addOption(std::string("db = " + conf.db), confDB);

	} while (menu->menu() > 0 );

};


void Console::confHost() {
    auto conf = _chat->conf();
    auto text = request(std::string("New host value ('" + conf.host + "') : "));
    if (text) conf.host = *text;
    _chat->setup(conf);
};


void Console::confUser() {
    auto conf = _chat->conf();
    auto text = request(std::string("New user value ('" + conf.user + "') : "));
    if (text) conf.user = *text;
    _chat->setup(conf);
};


void Console::confPasswd() {
    auto conf = _chat->conf();
    auto text = request(std::string("New passwd value ('" + conf.passwd + "') : "));
    if (text) conf.passwd = *text;
    _chat->setup(conf);
};


void Console::confDB() {
    auto conf = _chat->conf();
    auto text = request(std::string("New db value ('" + conf.db + "') : "));
    if (text) conf.db = *text;
    _chat->setup(conf);
};


void Console::confTry() {
    _chat->disconnect();
    _chat->connect();
};
