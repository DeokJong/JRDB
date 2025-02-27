#pragma once
#include "exception/fileException/FileException.h"

class FileNotFoundException : public FileException
{
public:
  explicit FileNotFoundException(const std::string &filePath)
    : FileException("FileNotFoundException: " + filePath) {}
};