#include "process_time.h"

// Получение текущего времени
std::string CurTime() {
  std::stringstream ss;
  time_t now = time(0);
  tm *ltm = localtime(&now);
  ss << ltm->tm_hour << ":" << ltm->tm_min << ":" << std::setfill('0') << std::setw(2) << ltm->tm_sec << " ";
  return ss.str();
}