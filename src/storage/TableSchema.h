#pragma once
#include <string>
#include <vector>

struct TableSchema {
	std::string tableName;
	std::vector<std::string> columnNames;
	std::vector<std::string> columnTypes;
};
