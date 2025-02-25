#pragma once
#include <sstream>
#include <vector>
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>

#include "tableSchema/TableSchema.h"

/*
* Lazy Loading
*/
class StorageEngine {

private:
	std::string dataDirectory;
	std::unordered_map<std::string, std::fstream> openTables;
	std::unordered_map<std::string, TableSchema> openTablesSchema;
public:
	StorageEngine(const std::string& directory);

	void createTable(const std::string& tableName, const std::vector<std::string>& columns, const std::vector<std::string>& types);
	void insertRecord(const std::string& tableName, const std::vector<std::string>& values);
	void deleteRecord(const std::string& tableName, const std::string& columnName, const std::string& value);
	void selectRecords(const std::string& tableName);
	void dropTable(const std::string& tableName);
	void loadTable(const std::string& tableName);
	void closeTable(const std::string& tableName);
	void closeAllTables();
};