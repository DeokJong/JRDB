#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

class TableSchema {
private:
	std::string tableName;
	std::vector<std::string> columnNames;
	std::vector<std::string> columnTypes;

public:
	TableSchema() = default;
	TableSchema(std::string tableName, std::vector<std::string> columnNames, std::vector<std::string> columnTypes)
	: tableName(tableName), columnNames(columnNames), columnTypes(columnTypes) {}
	void saveSchemaToFile(const std::string& directory);

	static TableSchema loadSchemaFromFile(const std::string& directory, const std::string& tableName);
};
