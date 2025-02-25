#pragma once
#include <string>
#include <fstream>
#include <unordered_map>

class StorageEngine {
private:
	std::string dataDirectory;
	std::unordered_map<std::string, std::fstream> openTables;

public:
	StorageEngine(const std::string& directory) : dataDirectory(directory) {}

	void createTable(const std::string& tableName, const std::vector<std::string>& columns);

	void insertRecord(const std::string& tableName, const std::vector<std::string>& values);

	void selectRecords(const std::string& tableName);

	void dropTable(const std::string& tableName);

	void loadTable(const std::string& tableName);
};
