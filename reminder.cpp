#include "reminder.hpp"

#include <iomanip>
#include <iostream>
#include <utility>

Reminder::Reminder(int argc, char** argv) {
	GetArgs(argc, argv);
	GetDate();
	ReadList();
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
		std::cout << "Очистка всех записей!" << std::endl;
		std::cout << "Продолжить? [д/н]: ";
		std::string answer;
		std::getline(std::cin, answer);
		std::cout << std::endl;
		if (answer == "д" || answer == "Д") {
			if (persons.size() != 0) {
				persons.clear();
				isChanged = true;
				std::cout << "Список напоминаний очищен!" << std::endl;
			} else {
				std::cout << "Список был пуст!" << std::endl;
			}
		} else if (answer == "н" || answer == "Н") {
			std::cout << "Очистка всех записей отменена!" << std::endl;
		} else {
			std::cout
				<< "Введен некорректный ответ. Очистка всех записей отменена!"
				<< std::endl;
		}
		returnValue = true;
	}
	int i = CheckArgs("-d");
	if (i != -1) {
		try {
			size_t recordNumber = std::stoi(args[i].substr(2));
			if (recordNumber > persons.size()) {
				throw std::out_of_range(
					"Указан номер записи для удаление больше числа записей в "
					"списке");
			}
			std::cout << "Удаление записи с номером " << recordNumber << "!"
					  << std::endl;

			size_t numberInOrder{1};
			typedef std::multimap<time_t, std::string>::const_iterator CI;
			CI p = persons.begin();
			for (; p != persons.end(); ++p) {
				if (numberInOrder == recordNumber) {
					break;
				}
				++numberInOrder;
			}
			std::cout << "-------------------------------\n";
			std::cout << numberInOrder << ". " << std::setfill('0')
					  << std::setw(2) << localtime(&p->first)->tm_mday << '.'
					  << std::setw(2) << localtime(&p->first)->tm_mon + 1
					  << '\t' << p->second << std::endl;
			std::cout << "-------------------------------\n";

			std::cout << "Продолжить? [д/н]: ";
			std::string answer;
			std::getline(std::cin, answer);
			std::cout << std::endl;
			if (answer == "д" || answer == "Д") {
				persons.erase(p);
				isChanged = true;
				std::cout << "Запись удалена!" << std::endl;
			} else if (answer == "н" || answer == "Н") {
				std::cout << "Удаление записи отменено!" << std::endl;
			} else {
				std::cout
					<< "Введен некорректный ответ. Удаление записи отменено!"
					<< std::endl;
			}
		} catch (std::invalid_argument const& ex) {
			std::cout << "Удаление записи отменено:" << std::endl;
			std::cout << "не правильный формат ключа (" << args[i] << ")"
					  << '\n';
		} catch (std::out_of_range const& ex) {
			std::cout << "Удаление записи отменено:" << std::endl;
			std::cout << ex.what() << " (" << args[i] << ")" << '\n';
		}
		returnValue = true;
	}
	if (CheckArgs("-a") != -1) {
		std::cout << std::endl
				  << "Добавление в список напоминаний" << std::endl;
		while (true) {
			std::cout << "Введите имя и Enter или Enter для завершения: "
					  << std::endl;
			std::string name;
			std::getline(std::cin, name);
			if (name.empty()) {
				std::cout << "Ввод завершен" << std::endl;
				break;
			}
			std::cout
				<< "Введите дату в формате ДД.ММ или Enter для завершения: "
				<< std::endl;
			std::string date;
			tm tm{};
			std::getline(std::cin, date);
			if (date.empty()) {
				std::cout << "Ввод завершен" << std::endl;
				break;
			}
			size_t posDot = date.find('.');
			if (posDot == date.npos) {
				std::cout << "Неверный формат. Ввод завершен" << std::endl;
				break;
			}
			tm.tm_mday = std::stoi(date.substr(0, posDot));
			tm.tm_mon = std::stoi(date.substr(posDot + 1)) - 1;
			time_t t = std::mktime(&tm);
			persons.insert(std::make_pair(t, name));
			isChanged = true;
		}
		returnValue = true;
	}
	if (CheckArgs("-i") != -1) {
		std::cout << std::endl << "Вывод всех записей" << std::endl;
		returnValue = true;
	}
	if (!returnValue) {
		std::cout << "Нет правильных ключей!" << std::endl;
	}
	if (isChanged) {
		if (WriteList()) {
			std::cout << "Список напоминаний записан на диск!" << std::endl;
		} else {
			std::cout << "Список напоминаний не удалось записать на диск!"
					  << std::endl;
			return EXIT_FAILURE;
		}
	}
	return returnValue;
}

void Reminder::GetDate() {
	curentDay = std::time(nullptr);
	tm* ltm = std::localtime(&curentDay);
	std::cout << std::endl;
	std::cout << "Сегодня " << std::setfill('0') << std::setw(2) << ltm->tm_mday
			  << '.' << std::setw(2) << 1 + ltm->tm_mon << '.'
			  << 1900 + ltm->tm_year << ':' << std::endl;
	tm tm{};
	tm.tm_mday = ltm->tm_mday;
	tm.tm_mon = ltm->tm_mon;
	curentDay = std::mktime(&tm);
}

bool Reminder::ReadList() {
	std::ifstream inFile{persons_storage_path + persons_storage_filename};
	if (!inFile.is_open()) {
		std::cout << "Файл с записями не найден!" << std::endl;
		return false;
	}
	std::string nameInList;
	time_t timeStamp{0};
	while (true) {
		inFile >> timeStamp;
		if (inFile.fail()) break;
		std::getline(inFile, nameInList, '\t');
		std::getline(inFile, nameInList);
		persons.insert(std::make_pair(timeStamp, nameInList));
	}
	inFile.close();
	return true;
}

bool Reminder::WriteList() {
	std::ofstream outFile{persons_storage_path + persons_storage_filename};
	if (!outFile.is_open()) {
		std::cout << "Ошибка! Файл не удалось открыть для записи!'\n'";
		return false;
	}
	typedef std::multimap<time_t, std::string>::const_iterator CI;
	for (CI p = persons.begin(); p != persons.end(); ++p) {
		outFile << p->first << '\t' << p->second << std::endl;
	}
	outFile.close();
	return true;
}

void Reminder::PrintList() const {
	std::cout << std::endl;
	if (persons.empty()) {
		std::cout << "Список напоминаний пуст" << std::endl;
		std::cout << std::endl;
		return;
	}
	std::cout << "------------------\nСписок напоминаний:\n------------------"
			  << std::endl;
	int numberInOrder{0};
	for (auto p = persons.cbegin(); p != persons.end(); ++p) {
		time_t t = p->first;
		tm* ltm = std::localtime(&t);
		++numberInOrder;
		std::cout << numberInOrder << ". " << std::setfill('0') << std::setw(2)
				  << ltm->tm_mday << '.' << std::setw(2) << ltm->tm_mon + 1
				  << '\t' << p->second << std::endl;
	}
	std::cout << std::endl;
}

void Reminder::CheckTodayNotify() const {
	typedef std::multimap<time_t, std::string>::const_iterator CI;
	std::pair<CI, CI> equal =
		persons.equal_range(curentDay);
	if (equal.first == persons.cend()) {
		std::cout << "Напоминаний на сегодня нет" << std::endl;
		std::cout << std::endl;
	} else {
		std::cout << "------------------\nНАПОМИНАНИЕ:\n------------------"
				  << std::endl;
		for (CI i = equal.first; i != equal.second; ++i) {
			std::cout << i->second << std::endl;
		}
		std::cout << std::endl;
	}
}
