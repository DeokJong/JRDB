#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

struct TableSchema {
	std::string tableName;
	std::vector<std::string> columnNames;
	std::vector<std::string> columnTypes;

	void saveSchemaToFile(const std::string& directory);

	static TableSchema loadSchemaFromFile(const std::string& directory, const std::string& tableName);
};
