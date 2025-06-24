#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "interaction.hpp"

TEST(Interaction, PrintDate) {
	// Arrange
	InteractionTerminal iteract;
	tm tm{};
	tm.tm_mday = 31;
	tm.tm_mon = 0;
	tm.tm_year = 125;
	std::string test_string{"\nСегодня 31.01.2025:\n"};
	std::ostringstream stream;

	// Act (Creation for this test)
	iteract.PrintDate(&tm, stream);

	// Assert
	ASSERT_FALSE(stream.str().empty());
	ASSERT_EQ(test_string.compare(stream.str()),0);
}

TEST(Interaction, ResponseRequest_Yes) {
	// Arrange
	InteractionTerminal iteract;
	std::string messages[2];
	messages[0] = "Очистка всех записей отменена!\n";
	messages[1] = "Введен некорректный ответ. Очистка всех записей отменена!\n";
	std::istringstream stream{"д"};

	// Act (Creation for this test)
	bool result = iteract.ResponseRequest(messages, stream);

	// Assert
	ASSERT_TRUE(result);
}

TEST(Interaction, ResponseRequest_No) {
	InteractionTerminal iteract;
	std::string messages[2];
	messages[0] = "Очистка всех записей отменена!\n";
	messages[1] = "Введен некорректный ответ. Очистка всех записей отменена!\n";
	std::istringstream stream{"Н"};

	bool result = iteract.ResponseRequest(messages, stream);

	ASSERT_FALSE(result);
}

TEST(Interaction, ResponseRequest_Incorrect) {
	InteractionTerminal iteract;
	std::string messages[2];
	messages[0] = "Очистка всех записей отменена!\n";
	messages[1] = "Введен некорректный ответ. Очистка всех записей отменена!\n";
	std::istringstream stream{"y"};

	bool result = iteract.ResponseRequest(messages, stream);

	ASSERT_FALSE(result);
}

TEST(Interaction, PrintMessage) {
	InteractionTerminal iteract;
	std::string test_string{"Test message\n"};
	std::ostringstream stream;

	iteract.PrintMessage(test_string.c_str(), stream);

	ASSERT_FALSE(stream.str().empty());
	ASSERT_EQ(test_string.compare(stream.str()),0);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
