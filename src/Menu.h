#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <vector>

//! Консольное меню
/*!
Выводит пункты меню по заданному перечню, запрашивает выбор пункта у пользователя.

Меню может воспроизводиться циклически 
(выходом назначается один из пунктов меню).

Настройки меню могут быть изменены во время работы.

*/
class Menu
{
public:

	typedef void (*respond)();

	long menu();

	void setHeader(const std::string& header, respond action = nullptr);
	void setFooter(const std::string& footer, respond action = nullptr);
	void setPromt(const std::string& promt, respond action = nullptr);

	long addOption(const std::string& label, respond action = nullptr);

	void markExit(long idx);

	void setFlagZeroOptionLast(bool flag);
	void setFlagRepeatMenu(bool flag);
	void setFlagExpectUserInput(bool flag);
	void setFlagPromtUntilValid(bool flag);

	bool isOptionIdxInMenu(long idx);

private:

	//! Пункт меню
	/*!
	* Задает текстовую метку и функцию, 
	вызываемую при выборе этого пункта.
	*/
	struct Entry
	{
		std::string lbl;
		respond act{ nullptr };
		void call();
		Entry(const std::string& lbl, respond act);
		Entry() = default;
	};

	bool _flagZeroOptionLast{ true };
	bool _flagPromtUntilValid{ true };
	bool _flagExpectUserInput{ true };
	bool _flagRepeatMenu{ false };

	long _idxOptionExit{ -1 };

	Entry _header;
	Entry _footer;
	Entry _promt;
	std::vector<Entry> _options;

	long promt();
	long input();
	void printOptions();
	void printOption(long idx);

	bool shouldRepeatMenu(long idxOptionSelected);

};
