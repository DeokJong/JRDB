#include "SerializeManager.h"

#include <memory>

#include "FileNotFoundException.h"
#include "FileReadException.h"

template <typename T>
requires std::is_base_of_v<std::ios, T>
void SerializeManager::fileErrorCheck(std::unique_ptr<T>& file, std::string filePath)
{
    if (!file->is_open()) {
        throw FileNotFoundException(filePath);
    }
    if (file->fail()) {
        throw FileReadException(filePath);
    }
}

template <>
TableSchema SerializeManager::loadFile<TableSchema>(std::string directory, std::string tableName)
{
  std::string filePath = directory + "/" + tableName + ".schema";

  auto file = std::make_unique<std::ifstream>(filePath);

  fileErrorCheck(file, filePath);

  std::string tableNameFromFile;
  std::getline(*file, tableNameFromFile);

  std::vector<std::string> columnNames;
  std::vector<std::string> columnTypes;
  std::string line;

  while(std::getline(*file, line))
  {
    std::string columnName;
    std::string columnType;

    std::istringstream iss(line);
    if(!(iss >> columnName >> columnType))
    {
      throw FileReadException(filePath);  
    }
    iss >> columnName >> columnType;
    columnNames.push_back(columnName);
    columnTypes.push_back(columnType);
  }

  return TableSchema(tableNameFromFile, columnNames, columnTypes);

}

template <>
std::unique_ptr<std::fstream> SerializeManager::loadFile<std::unique_ptr<std::fstream>>(std::string directory, std::string tableName)
{
  std::string filePath = directory + "/" + tableName + ".db";

  auto file = std::make_unique<std::fstream>(filePath);

  fileErrorCheck(file, filePath);

  return file;
}


