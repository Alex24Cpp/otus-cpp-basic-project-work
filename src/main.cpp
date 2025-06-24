#include <iostream>

#include "interaction.hpp"
#include "reminder.hpp"

int main(int argc, char** argv) {
	try {
		InteractionTerminal interTerm;
		Reminder reminder(&interTerm);

		reminder.Run(argc, argv);
		reminder.CheckTodayNotify();
		if (reminder.ChoiceAction()) {
			reminder.PrintList();
		}
	} catch (std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	return 0;
}