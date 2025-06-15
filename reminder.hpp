#pragma once
#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include <vector>

class Reminder {
private:
	void GetArgs(int argc, char** argv);
	int CheckArgs(std::string arg);
	void GetDate();
	bool ReadList();
	bool WriteList();

	std::multimap<std::time_t, std::string> persons;
	const std::string persons_storage_filename = "persons.txt";
	std::string persons_storage_path;
	bool isChanged{false};
	time_t curentDay{};
	std::vector<std::string> args;
public:
	Reminder(int argc, char** argv);
	void CheckTodayNotify() const;
	bool ChoiceAction();
	void PrintList() const;
};
