#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "mes.h"

class Elevator {
 public:
  Elevator() {
    // получение ключа
    // нужно имя заведомо существующего файла
    // и символ
    if ((key = ftok("Elevator.cpp", 'A')) < 0) {
      printf("Can't get key\n");
      exit(1);
    }

    // создание очереди сообщений
    if ((mesid = msgget(key, PERM | IPC_CREAT)) < 0) {
      printf("Can't create message's queue\n");
      exit(1);
    }
  }

  ~Elevator() {
    if (msgctl(mesid, IPC_RMID, 0) < 0) {
      printf("Can't delete queue\n");
      exit(1);
    }
  }

  void MakeMessage(const char[MAXBUF], long type) {
    privet.type = type;
    lng = sprintf(privet.buf, "Thank's\n");
  }

  void SendMessage() {
    if (msgsnd(mesid, (void*)&privet, lng, 0) < 0) {
      printf("Can't write message\n");
      exit(1);
    } else {
      printf("The message is not valid\n");
      exit(1);
    }
  }

  void ReadWithFreeze(long type) {
    privet.type = type;
    n = msgrcv(mesid, &privet, sizeof(Message), privet.type, 0);
  }

  void ReadWithoutFreeze(long type) {
    privet.type = type;
    n = msgrcv(mesid, &privet, sizeof(Message), privet.type, IPC_NOWAIT);
  }

  void Start() {
    while (!floor_dest.empty()) {
      int passengers_floor;
      int passengers_direction;
      errno = 0;
      while (errno == 0) {
        ReadWithFreeze(4 + mov_direction);
        BuffToString(passengers_floor, passengers_direction);
        floor_dest.push_back(passengers_floor);
      }
      if (std::find(floor_dest.begin(), floor_dest.end(), cur_floor) !=
          floor_dest.end()) {
        std::string temp = std::to_string(cur_floor);
        MakeMessage(temp.c_str(), 1);
      }
    }
  }

 private:
  // сообщение
  Message privet;
  // ключ для создания системного ресурса
  key_t key;
  // дескриптор очереди сообщений
  int mesid;
  int lng, n;

  int cur_floor = 0;
  int mov_direction = 0;
  std::vector<int> floor_dest = {0};

  void BuffToString(int& pf, int& pd) {
    std::stringstream ss;
    std::string temp(privet.buf);
    ss << temp;
    ss >> pf;
    ss >> pd;
  }
};
