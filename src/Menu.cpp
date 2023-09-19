#include "Menu.h"

#include <iostream>
#include <string>

long Menu::menu()
{
	long idxOptionSelected{ -1 };

	do {
		std::cout << std::endl;
		_header.call();
		idxOptionSelected = promt();
		_options[idxOptionSelected].call();
		_footer.call();
	} while (shouldRepeatMenu(idxOptionSelected));


	return idxOptionSelected;
};


long Menu::promt() {

	long idxOptionSelected{ -1 };

	do {
		printOptions();
		_promt.call();

		idxOptionSelected = input();

		if (!isOptionIdxInMenu(idxOptionSelected))
		{
			std::cout << "#!ERR: unvalid menu option selection" << std::endl;
		};

	} while (_flagPromtUntilValid && (!isOptionIdxInMenu(idxOptionSelected)));

	return idxOptionSelected;
};


bool Menu::isOptionIdxInMenu(long idx) { return ((idx >= 0) && (idx < _options.size())); };


long Menu::input()
{
	long idxOptionSelected{ -1 };
	std::cout << "# Select menu number : ";
	std::cin >> idxOptionSelected;
	std::cin.ignore(32767, '\n');
	return idxOptionSelected;
};


bool Menu::shouldRepeatMenu(long idxOptionSelected) {
	return (_flagRepeatMenu && (_idxOptionExit != idxOptionSelected));
};


long Menu::addOption(const std::string& label, respond action)
{
	_options.push_back(Menu::Entry(label, action));
	return (static_cast<long>(_options.size()) - 1);
};


void Menu::setHeader(const std::string& header, respond action)
{
	_header.lbl = header;
	_header.act = action;
};


void Menu::setFooter(const std::string& footer, respond action)
{
	_footer.lbl = footer;
	_footer.act = action;
};


void Menu::setPromt(const std::string& promt, respond action)
{
	_promt.lbl = promt;
	_promt.act = action;
};


void Menu::printOptions()
{

	long numOptions = static_cast<long>(_options.size());

	if (numOptions > 0) {
		if (_flagZeroOptionLast) {
			for (long idx = 1; idx < numOptions; ++idx) { printOption(idx); };
			printOption(0);
		}
		else {
			for (long idx = 0; idx < numOptions; ++idx) { printOption(idx); };
		};
	};

};


void Menu::printOption(long idx)
{
	std::cout << idx << "): " << (_options[idx]).lbl << std::endl;
};


void Menu::setFlagZeroOptionLast(bool flag) { _flagZeroOptionLast = flag; };

void Menu::setFlagRepeatMenu(bool flag) { _flagRepeatMenu = flag; };

void Menu::setFlagExpectUserInput(bool flag) { _flagExpectUserInput = flag; };

void Menu::setFlagPromtUntilValid(bool flag) { _flagPromtUntilValid = flag; };

void Menu::markExit(long idx) { _idxOptionExit = idx; };


Menu::Entry::Entry(const std::string& label, respond action)
	: lbl(label), act(action) {};


void Menu::Entry::call()
{
	if (!lbl.empty()) { std::cout << lbl << std::endl; };
	if (act) { (*act)(); };
};

