#pragma once
#include <filesystem>
#include <fstream>
#include <string>

#include "tableSchema/TableSchema.h"

class SerializeManager {
  private:
  template <typename T>
  requires std::is_base_of_v<std::ios, T>
  static void fileErrorCheck(std::unique_ptr<T>& file, std::string filePath);
  public:
      template<typename T>
      static T loadFile(std::string directory, std::string tableName);
      
  };