#include "SerializeManager.h"

#include <memory>

#include "exception/fileException/FileNotFoundException.h"
#include "exception/fileException/FileReadException.h"

template <typename T>
requires std::is_base_of_v<std::ios, T>
void SerializeManager::fileErrorCheck(std::unique_ptr<T>& file, std::string filePath)
{
  std::filesystem::path path(filePath);
    if (!file->is_open()) {
        throw FileNotFoundException(std::filesystem::absolute(path));
    }
    if (file->fail()) {
        throw FileReadException(std::filesystem::absolute(path));
    }
}

TableSchema SerializeManager::loadFile(std::string directory, std::string tableName)
{
  std::string filePath = directory + "/" + tableName + ".schema";
  std::filesystem::path path(filePath);
  auto file = std::make_unique<std::ifstream>(std::filesystem::absolute(path));

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

std::unique_ptr<std::fstream> SerializeManager::loadFileStream(std::string directory, std::string tableName)
{
  std::string filePath = directory + "/" + tableName + ".db";

  auto file = std::make_unique<std::fstream>(filePath);

  fileErrorCheck(file, filePath);

  return file;
}


