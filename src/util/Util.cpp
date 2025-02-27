#include "Util.h"
#include <ctime>
#include <iomanip>

std::string Util::getCurrentTimeString()
{
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);
  std::stringstream ss;
  ss << std::put_time(&tm, "%Y-%m-%d %H-%M-%S");
  std::string time = ss.str();
  return time;
}