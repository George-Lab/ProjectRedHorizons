#define MAXBUF 100
#define PERM 0666

// Структура для передачи сообщений
typedef struct msgbuffer {
  // тип сообщения для чтения или записи
  long type;
  // для содержания сообщения
  char buf[MAXBUF];
  // могут быть и еще другие поля
} Message;