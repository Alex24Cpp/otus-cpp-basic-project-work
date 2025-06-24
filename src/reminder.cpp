#include "reminder.hpp"

#include <iomanip>
#include <iostream>
#include <utility>

#include "interaction.hpp"

Reminder::Reminder(IInteraction* inter) : pInter{inter} {
	personsStorege = Storage(inter);
}

void Reminder::Run(int argc, char** argv) {
	GetArgs(argc, argv);
	GetDate();
	pInter->PrintDate(pLtm);
	personsStorege.ReadList(persons_storage_path + persons_storage_filename);
}

void Reminder::GetArgs(int argc, char** argv) {
	persons_storage_path = argv[0];
	persons_storage_path.erase(persons_storage_path.find_last_of("/") + 1);
	for (int i = 1; i < argc; i++) {
		args.emplace_back(argv[i]);
	}
}

int Reminder::CheckArgs(std::string arg) {
	for (size_t i = 0; i < args.size(); ++i) {
		if (args[i].contains(arg)) {
			return i;
		}
	}
	return -1;
}

bool Reminder::ChoiceAction() {
	bool returnValue{false};
	if (args.empty()) {
		return returnValue;
	}
	if (CheckArgs("-c") != -1) {
		pInter->PrintMessage("Очистка всех записей!\n");
		std::string messages[2];
		messages[0] = "Очистка всех записей отменена!\n";
		messages[1] =
			"Введен некорректный ответ. Очистка всех записей отменена!\n";
		if (pInter->ResponseRequest(messages)) {
			if (personsStorege.GetListSize() != 0) {
				personsStorege.ClearList();
				isChanged = true;
				pInter->PrintMessage("Список напоминаний очищен!\n");
			} else {
				pInter->PrintMessage("Список был пуст!\n");
			}
		}
		returnValue = true;
	}
	int i = CheckArgs("-d");
	if (i != -1) {
		try {
			size_t recordRemoveNumber = std::stoi(args[i].substr(2));
			if (recordRemoveNumber > personsStorege.GetListSize()) {
				throw std::out_of_range(
					"Указан номер записи для удаление больше числа записей в "
					"списке");
			}
			std::pair<time_t, std::string> record =
				personsStorege.GetRecordByNumber(recordRemoveNumber);
			std::ostringstream stream{"Удаление записи с номером ",
									  std::ios_base::ate};
			stream << recordRemoveNumber << "!\n";
			stream << "-------------------------------\n";
			stream << recordRemoveNumber << ". " << std::setfill('0')
				   << std::setw(2) << localtime(&record.first)->tm_mday << '.'
				   << std::setw(2) << localtime(&record.first)->tm_mon + 1
				   << '\t' << record.second
				   << "\n-------------------------------\n";
			pInter->PrintMessage(stream.str().c_str());

			std::string messages[3];
			messages[0] = "Удаление записи отменено!\n";
			messages[1] =
				"Введен некорректный ответ. Удаление записи отменено!\n";
			if (pInter->ResponseRequest(messages)) {
				personsStorege.RomoveRecord(recordRemoveNumber);
				isChanged = true;
				pInter->PrintMessage("Запись удалена!\n");
			}
		} catch (std::invalid_argument const& ex) {
			std::ostringstream stream{
				"Удаление записи отменено:\nне правильный формат ключа (",
				std::ios_base::ate};
			stream << args[i] << ")\n";
			pInter->PrintMessage(stream.str().c_str());
		} catch (std::out_of_range const& ex) {
			std::ostringstream stream{"Удаление записи отменено:\n",
									  std::ios_base::ate};
			stream << ex.what() << " (" << args[i] << ")" << '\n';
			pInter->PrintMessage(stream.str().c_str());
		}
		returnValue = true;
	}
	if (CheckArgs("-a") != -1) {
		pInter->PrintMessage("\nДобавление в список напоминаний\n");
		isChanged = personsStorege.AddRecord();
		returnValue = true;
	}
	if (CheckArgs("-i") != -1) {
		pInter->PrintMessage("\nВывод всех записей\n");
		returnValue = true;
	}
	if (!returnValue) {
		pInter->PrintMessage("Нет правильных ключей!\n");
	}
	if (isChanged) {
		if (personsStorege.WriteList(persons_storage_path +
									 persons_storage_filename)) {
			pInter->PrintMessage("Список напоминаний записан на диск!\n");
		} else {
			pInter->PrintMessage(
				"Список напоминаний не удалось записать на диск!\n");
			return EXIT_FAILURE;
		}
	}
	return returnValue;
}

void Reminder::GetDate() {
	curentDay = std::time(nullptr);
	pLtm = std::localtime(&curentDay);
	tm tm{};
	tm.tm_mday = pLtm->tm_mday;
	tm.tm_mon = pLtm->tm_mon;
	curentDay = std::mktime(&tm);
}

void Reminder::PrintList() const {
	if (personsStorege.IsEmpty()) {
		pInter->PrintMessage("\nСписок напоминаний пуст\n\n");
		return;
	}
	std::ostringstream outStream{
		"\n------------------\nСписок напоминаний:\n------------------\n",
		std::ios_base::ate};
	for (size_t numberInOrder{1}; numberInOrder <= personsStorege.GetListSize();
		 ++numberInOrder) {
		std::pair<time_t, std::string> record =
			personsStorege.GetRecordByNumber(numberInOrder);
		time_t t = record.first;
		tm* ltm = std::localtime(&t);
		outStream << numberInOrder << ". " << std::setfill('0') << std::setw(2)
				  << ltm->tm_mday << '.' << std::setw(2) << ltm->tm_mon + 1
				  << '\t' << record.second << "\n";
	}
	outStream << "\n";
	pInter->PrintMessage(outStream.str().c_str());
}

void Reminder::CheckTodayNotify() const {
	std::vector<std::pair<time_t, std::string>> records =
		personsStorege.CheckRecordsByDate(curentDay);
	if (records.empty()) {
		pInter->PrintMessage("Напоминаний на сегодня нет\n\n");
	} else {
		pInter->PrintMessage(
			"------------------\nНАПОМИНАНИЕ:\n------------------\n");
		for (auto record : records) {
			pInter->PrintMessage(record.second.c_str());
			pInter->PrintMessage("\n");
		}
		pInter->PrintMessage("\n");
	}
}
