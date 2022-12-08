#include <fstream>

#include "message_queue.h"

#define BUFF_SIZE 4096

class Lift {
 public:
  Lift();
  ~Lift();

  int read_nowait(int mess_type);

  int read_wait(int mess_type);

  void send_mess(int mess_type, const char* text);

  // Отправка сообщения в лог
  void Log(std::string s);

  // Обновить лог файл
  // Выгрузить из потока вывода в лог
  void UpdateLog();

  void pass_pressed_button();

  void pass_entered();

  void pass_exited();

  bool NobodyWaitingOutside();

  bool NobodyWaitingInside();

 private:
  // Идентификатор очереди сообщений
  int mesid;

  // Структура для обмена сообщениями
  Message Mess;

  // Поток для вывода в log.txt
  std::ofstream logfile;

  // Количество пассажиров, ждущих
  // прибытия лифта снаружи
  int waiting;

  // Количество пассажирова, ждущих
  // нужного этажа внутри лифта
  int inside;
};
