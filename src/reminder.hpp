#pragma once
#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "iinteraction.hpp"
#include "storage.hpp"

class Reminder {
public:
	Reminder(IInteraction* inter);
	void CheckTodayNotify() const;
	bool ChoiceAction();
	void PrintList() const;
	void Run(int argc, char** argv);
private:
	void GetArgs(int argc, char** argv);
	int CheckArgs(std::string arg);
	void GetDate();

	Storage personsStorege;
	const std::string persons_storage_filename = "persons.txt";
	std::string persons_storage_path;
	bool isChanged{false};
	time_t curentDay{};
	tm* pLtm{nullptr};
	std::vector<std::string> args;
	IInteraction* pInter;
};
