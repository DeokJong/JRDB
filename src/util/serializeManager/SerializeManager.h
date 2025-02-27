#pragma once
#include <filesystem>
#include <fstream>
#include <string>

#include "storage/tableSchema/TableSchema.h"

class SerializeManager
{
private:
  template <typename T>
  requires std::is_base_of_v<std::ios, T>
  static void fileErrorCheck(std::unique_ptr<T> &file, std::string filePath);

public:
  static TableSchema loadFile(std::string directory, std::string tableName);
  static std::unique_ptr<std::fstream> loadFileStream(std::string directory, std::string tableName);
};