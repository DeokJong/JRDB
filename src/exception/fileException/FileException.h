#pragma once
#include <exception>
#include <string>
#include <vector>

class FileException : public std::exception
{
private:
  void saveLogFile() const;

protected:
  std::string message;
  std::vector<std::string> stackTrace;
  void captureStackTrace();

public:
  explicit FileException(const std::string &msg);
  const char *what() const noexcept override;
  std::vector<std::string> getStackTrace() const;
  void printStackTrace() const;
};