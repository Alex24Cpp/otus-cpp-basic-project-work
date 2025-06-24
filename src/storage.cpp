#include "storage.hpp"

#include <fstream>

Storage::Storage(IInteraction* inter) : pInter{inter} {
}

bool Storage::AddRecord(std::istream& inStream) {
	bool isChanged{false};
	while (true) {
		pInter->PrintMessage(
			"Введите имя и Enter или Enter для завершения: \n");
		std::string name;
		std::getline(inStream, name);
		if (name.empty()) {
			pInter->PrintMessage("Ввод завершен\n");
			break;
		}
		pInter->PrintMessage(
			"Введите дату в формате ДД.ММ или Enter для завершения: \n");
		std::string date;
		tm tm{};
		std::getline(inStream, date);
		if (date.empty()) {
			pInter->PrintMessage("Ввод завершен\n");
			break;
		}
		size_t posDot = date.find('.');
		if (posDot == date.npos) {
			pInter->PrintMessage("Неверный формат. Ввод завершен\n");
			break;
		}
		tm.tm_mday = std::stoi(date.substr(0, posDot));
		tm.tm_mon = std::stoi(date.substr(posDot + 1)) - 1;
		time_t t = std::mktime(&tm);
		persons.insert(std::make_pair(t, name));
		isChanged = true;
	}
	return isChanged;
}

void Storage::RomoveRecord(size_t numberRemove) {
	size_t numberInOrder{1};
	CI p = persons.begin();
	for (; p != persons.end(); ++p) {
		if (numberInOrder == numberRemove) {
			break;
		}
		++numberInOrder;
	}
	persons.erase(p);
}

bool Storage::ClearList() {
	bool returnValue{false};
	if (persons.size() != 0) {
		persons.clear();
		returnValue = true;
	}
	return returnValue;
}

bool Storage::ReadList(std::string storageFullPath) {
	std::ifstream inFile{storageFullPath};
	if (!inFile.is_open()) {
		pInter->PrintMessage("Файл с записями не найден!\n");
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

bool Storage::WriteList(std::string storageFullPath) {
	std::ofstream outFile{storageFullPath};
	if (!outFile.is_open()) {
		pInter->PrintMessage("Ошибка! Файл не удалось открыть для записи!\n");
		return false;
	}
	typedef std::multimap<time_t, std::string>::const_iterator CI;
	for (CI p = persons.begin(); p != persons.end(); ++p) {
		outFile << p->first << '\t' << p->second << std::endl;
	}
	outFile.close();
	return true;
}

size_t Storage::GetListSize() const {
	return persons.size();
}

bool Storage::IsEmpty() const {
	return persons.empty();
}

std::pair<time_t, std::string> Storage::GetRecordByNumber(size_t number) const {
	size_t numberInOrder{1};
	CI p = persons.begin();
	for (; p != persons.end(); ++p) {
		if (numberInOrder == number) {
			break;
		}
		++numberInOrder;
	}
	return std::pair<time_t, std::string>{p->first, p->second};
}

std::vector<std::pair<time_t, std::string>> Storage::CheckRecordsByDate(
	time_t day) const {
	std::vector<std::pair<time_t, std::string>> records;
	std::pair<CI, CI> equal = persons.equal_range(day);
	if (equal.first != persons.cend()) {
		for (CI i = equal.first; i != equal.second; ++i) {
			records.emplace_back(*i);
		}
	}
	return records;
}
