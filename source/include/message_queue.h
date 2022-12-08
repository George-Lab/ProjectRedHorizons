#ifndef MESSAGE_QUEUE
#define MESSAGE_QUEUE

#include <sstream>
#include <string>

#define MAXBUF 4096

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
int read_message_wait(int mesid, Message* mes, int mess_type);

// Пытается прочитать сообщение, если сообщения нет
// программа продолжает исполнение
int read_message_nowait(int mesid, Message* mes, int mess_type);

void send_message(int mesid, Message* mes, const char* text, int mess_type);

void delete_message_queue(int mesid);

#endif