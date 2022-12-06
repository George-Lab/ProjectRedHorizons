#ifndef MESSAGE_QUEUE
#define MESSAGE_QUEUE

#include <iostream>
#include <sstream>
#include <string>

#include "shared_values.h"

// Структура для передачи сообщений
typedef struct MessageBuf {
  // тип сообщения для чтения или записи
  long type;

  // для содержания сообщения
  char buf[MAXBUF];
} Message;

int create_message_queue(const char* filename);

int get_message_queue_id(const char* filename);

// Пытается прочитать сообщение, если сообщения нет
// ожидает появления сообщения
int read_message_wait(int mesid, Message* mes);

// Пытается прочитать сообщение, если сообщения нет
// программа продолжает исполнение
int read_message_nowait(int mesid, Message* mes);

void send_message(int mesid, Message* mes, const char* text);

// Отправка сообщения в log файл и вывод в окно терминала
void send_log(std::ostream& os, std::stringstream& sstr, std::string& mes_str,
              Message* mes_log, int mesid_log);

void delete_message_queue(int mesid);

#endif