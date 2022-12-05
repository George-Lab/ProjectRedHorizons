#include "message_queue.h"

#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "shared_values.h"

int create_message_queue(const char* filename) {
  key_t key;
  key = ftok(filename, 1);
  if (key == MESQUEUE_RESULT_ERROR) {
    return MESQUEUE_RESULT_ERROR;
  }

  // создание очереди сообщений
  int mesid = msgget(key, 0666 | IPC_CREAT);
  Message extractor;
  extractor.type = 0;
  while (true) {
    if (read_message_nowait(mesid, &extractor) == -1) {
      break;
    }
  }

  return mesid;
}

int get_message_queue_id(const char* filename) {
  key_t key;
  key = ftok(filename, 1);
  if (key == MESQUEUE_RESULT_ERROR) {
    return MESQUEUE_RESULT_ERROR;
  }

  // получение дескриптора очереди сообщений
  return msgget(key, 0);
}

int read_message_wait(int mesid, Message* mes) {
  // чтение из очереди сообщений
  // если сообщений в очереди нет, программа в ожидании
  int n = msgrcv(mesid, mes, sizeof(Message), mes->type, 0);

  // для печати - 0 в конце
  mes->buf[n] = '\0';

  return 1;
}

int read_message_nowait(int mesid, Message* mes) {
  // чтение из очереди сообщений
  // если сообщений в очереди нет, программа продолжает исполнение
  int n = msgrcv(mesid, mes, sizeof(Message), mes->type, IPC_NOWAIT);
  if (errno == ENOMSG) {
    return MESQUEUE_RESULT_ERROR;
  }

  // для печати - 0 в конце
  mes->buf[n] = '\0';

  return 1;
}

void send_message(int mesid, Message* mes, const char* text) {
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