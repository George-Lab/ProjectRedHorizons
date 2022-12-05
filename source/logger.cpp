#include <fcntl.h>
#include <semaphore.h>

#include <fstream>
#include <sstream>
#include <iostream>

#include "include/message_queue.h"

int main() {
  // Получение очереди сообщений для logger'а
  int mesid_log = get_message_queue_id(FILENAME_LOGGER);
  if (mesid_log < 0) {
    printf("Can't recieve message's queue\n");
    exit(1);
  }

  // Структура для обмена сообщениями
  Message mes_log;
  mes_log.type = 1;

  // Открытие файла log'а
  std::ofstream logfile;
  logfile.open(LOG_PATH);
  if(!logfile) {
    std::cerr << "Couldn't open the file\n";
    exit(1);
  }

  while (true) {
    read_message_wait(mesid_log, &mes_log);
    std::cout << mes_log.buf;
    logfile << mes_log.buf;
    logfile.flush();
  }

  logfile.close();

    return 0;
}