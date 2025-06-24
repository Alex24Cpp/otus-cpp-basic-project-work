#include "interaction.hpp"

#include <iomanip>
#include <iostream>

void InteractionTerminal::PrintDate(tm* pTm, std::ostream& outStream) const {
	outStream << "\nСегодня " << std::setfill('0') << std::setw(2)
			  << pTm->tm_mday << '.' << std::setw(2) << 1 + pTm->tm_mon << '.'
			  << 1900 + pTm->tm_year << ":\n";
}

bool InteractionTerminal::ResponseRequest(std::string* messages,
										  std::istream& inStream) const {
	bool returnValue{false};

	std::cout << "Продолжить? [д/н]: ";
	std::string answer;
	std::getline(inStream, answer);
	std::cout << std::endl;
	if (answer == "д" || answer == "Д") {
		returnValue = true;
	} else if (answer == "н" || answer == "Н") {
		std::cout << messages[0];
	} else {
		std::cout << messages[1];
	}
	return returnValue;
}

void InteractionTerminal::PrintMessage(const char* message,
									   std::ostream& outStream) const {
	outStream << message;
}
