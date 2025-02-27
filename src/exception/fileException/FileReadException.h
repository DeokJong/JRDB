#pragma once
#include "FileException.h"

class FileReadException : public FileException
{
public:
  explicit FileReadException(const std::string &filePath)
    : FileException("FileReadException: " + filePath) {}
};