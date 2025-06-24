#pragma once
#include <ctime>
#include <iostream>
#include <map>
#include <string>

// интерфейс взаимодействия
class IInteraction {
public:
	virtual void PrintDate(tm* pTm,
						   std::ostream& outStream = std::cout) const = 0;
	virtual bool ResponseRequest(std::string* messages,
								 std::istream& inStream = std::cin) const = 0;
	virtual void PrintMessage(const char* message,
							  std::ostream& outStream = std::cout) const = 0;
	virtual ~IInteraction() = default;
};