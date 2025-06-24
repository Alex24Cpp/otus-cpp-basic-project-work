#pragma once
#include "iinteraction.hpp"

class InteractionTerminal : public IInteraction {
public:
	void PrintDate(tm* pTm, std::ostream& outStream = std::cout) const override;
	bool ResponseRequest(std::string* messages,
						 std::istream& inStream = std::cin) const override;
	void PrintMessage(const char* message,
					  std::ostream& outStream = std::cout) const;
};
