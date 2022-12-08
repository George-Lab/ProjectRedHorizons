#include "message_queue.h"

#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <iostream>

int create_message_queue(const char* filename) {
  key_t key;
  key = ftok(filename, 1);
  if (key == -1) {
    return -1;
  }

  // создание очереди сообщений
  int mesid = msgget(key, 0666 | IPC_CREAT);
  if (mesid < 0) {
    std::cerr << "Can't create message queue!\n";
    exit(-1);
  }

  return mesid;
}

int get_message_queue_id(const char* filename) {
  key_t key;
  key = ftok(filename, 1);
  if (key == -1) {
    return -1;
  }

  // получение дескриптора очереди сообщений
  int mesid = msgget(key, 0);
    if (mesid < 0) {
    std::cerr << "Can't access message queue!\n";
    exit(-1);
  }

  return mesid;
}

int read_message_wait(int mesid, Message* mes, int mess_type) {
  mes->type = mess_type;
  
  // чтение из очереди сообщений
  // если сообщений в очереди нет, программа в ожидании
  int n = msgrcv(mesid, mes, sizeof(Message), mes->type, 0);

  // для печати - 0 в конце
  mes->buf[n] = '\0';

  return 1;
}

int read_message_nowait(int mesid, Message* mes, int mess_type) {
  mes->type = mess_type;
  
  // чтение из очереди сообщений
  // если сообщений в очереди нет, программа продолжает исполнение
  errno = 0;
  int n = msgrcv(mesid, mes, sizeof(Message), mes->type, IPC_NOWAIT);
  if (errno == ENOMSG) {
    return -1;
  }

  // для печати - 0 в конце
  mes->buf[n] = '\0';

  return 1;
}

void send_message(int mesid, Message* mes, const char* text, int mess_type) {
  mes->type = mess_type;;
  
  // Подготовка нового сообщения для отправки
  int lng = sprintf(mes->buf, "%s", text);

  // отправка сообщения
  if (msgsnd(mesid, (void*)mes, lng, 0) < 0) {
    printf("Can't write message\n");
    exit(1);
  }
}

void delete_message_queue(int mesid) {
  if (msgctl(mesid, IPC_RMID, 0) < 0) {
    printf("Can't delete queue\n");
    exit(1);
  }
}