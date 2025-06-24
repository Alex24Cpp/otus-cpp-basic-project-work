#pragma once
#include <map>
#include <vector>

#include "iinteraction.hpp"

typedef std::multimap<time_t, std::string>::const_iterator CI;

class Storage {
public:
	Storage() = default;
	Storage(IInteraction *inter);
	bool AddRecord(std::istream &inStream = std::cin);
	void RomoveRecord(size_t numberRemove);
	bool ClearList();
	bool ReadList(std::string storageFullPath);
	bool WriteList(std::string storageFullPath);
	size_t GetListSize() const;
	bool IsEmpty() const;
	std::pair<time_t, std::string> GetRecordByNumber(size_t number) const;
	std::vector<std::pair<time_t, std::string>> CheckRecordsByDate(
		time_t day) const;
private:
	IInteraction *pInter;
	std::multimap<std::time_t, std::string> persons;
};
