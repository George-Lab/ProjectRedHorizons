#include "message_queue.h"

class Actor {
 public:
  Actor();
  ~Actor() = default;

  void read_nowait(int mess_type);

  void read_wait(int mess_type);

  void send_mess(int mess_type, const char* text);

 private:
  // Идентификатор очереди сообщений
  int mesid;

  // Структура для обмена сообщениями
  Message Mess;
};