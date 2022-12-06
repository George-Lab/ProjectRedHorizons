#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

#include "include/message_queue.h"
#include "include/shared_memory.h"
#include "include/process_time.h"

int main() {
  // Получение доступа к семафору,
  // контролирующему доступ к 
  // разделяемой памяти
  sem_t* sem_shmem = sem_open(SEM_SHMEM_BLOCK, 0);
  if (sem_shmem == SEM_FAILED) {
    perror("sem_open/sem_shmem");
    exit(EXIT_FAILURE);
  }

  // Получение доступа к области разделяемой памяти
  int* data = attach_memory_block(FILENAME, BLOCK_SIZE);
  if (data == NULL) {
    std::cout << "ERROR: couldn't get block\n";
    return -1;
  }

  // Получение доступа к очереди сообщений
  // для связи с lift
  int mesid = get_message_queue_id(FILENAME);
  if (mesid < 0) {
    printf("Can't recieve message's queue\n");
    exit(1);
  }

  // Получение доступа очереди сообщений для
  // для связи с logger'ом
  int mesid_log = get_message_queue_id(FILENAME_LOGGER);
  if (mesid_log < 0) {
    printf("Can't recieve message's queue\n");
    exit(1);
  }

  // Структуры для обмена сообщениями
  // с lift и logger
  Message mes;
  Message mes_log;
  mes_log.type = 1;

  // Для записи сообщений в log
  std::string str_mes;
  std::stringstream sstr;

  // Выбор этажа, где появился очередной житель
  // и этажа, куда этот житель направляется
  srand((unsigned)time(0));
  int spawn_floor = (rand() % 10) + 1;
  int dest_floor;
  do {
    dest_floor = (rand() % 10) + 1;
  } while (dest_floor == spawn_floor);

  // Житель нажимает на кнопку
  sstr << CurTime() << getpid() << ": I want to get from floor " << spawn_floor
       << " to floor " << dest_floor << std::endl;
  send_log(std::cout, sstr, str_mes, &mes_log, mesid_log);

  sstr << CurTime() << getpid() << ": Pressing button on the " << spawn_floor << " floor\n";
  send_log(std::cout, sstr, str_mes, &mes_log, mesid_log);

  // Определяем, в каком направлении будет двигаться житель
  int direction;
  if (dest_floor > spawn_floor) {
    direction = 1;
  } else {
    direction = -1;
  }

  // Житель нажимает кнопку, при этом значение ячейки блока памяти,
  // хранящей количество жителей, которые хотят хотят ехать в заданном
  // направлении на данном этаже, увеличивается на 1
  sem_wait(sem_shmem);
  data[2 * spawn_floor + (1 + direction) / 2] += 1;
  if (spawn_floor > data[1]) {
    data[1] = spawn_floor;
  }
  if (spawn_floor < data[0] || data[0] == 0) {
    data[0] = spawn_floor;
  }
  sem_post(sem_shmem);

  // Отправляем сообщение  тапа 1 процессу lift, чтобы
  // сообщить, что была нажата кнопка вызова лифта
  mes.type = 1;
  send_message(mesid, &mes, "UP/DOWN");

  // Ожидание лифта
  mes.type = 2 + (1 + direction) / 2;
  read_message_wait(mesid, &mes);

  // Вход в лифт
  sstr << CurTime() << getpid() << ": Entering lift\n";
  send_log(std::cout, sstr, str_mes, &mes_log, mesid_log);

  sem_wait(sem_shmem);
  // Житель нажимает кнопку этажа, на который он хочет попасть
  data[2 * MAX_FLOOR + dest_floor + 1] += 1;

  // Так как житель зашел в лифт, количество людей, ожидающих
  // лифт на этом этаже уменьшилось на 1
  data[2 * spawn_floor + (1 + direction) / 2] -= 1;

  sleep(PASSENGER_ENTER_TIME);

  // Проверяем, нужно ли обновить значения максимального и минимального
  // номеров этажей, которые должен посетить лифт
  if (direction == 1 && dest_floor > data[1]) {
    data[1] = dest_floor;
  }
  if (direction == -1 && (dest_floor < data[0] || data[0] == 0)) {
    data[0] = dest_floor;
  }

  // Если последний из ожидающих зашел в лифт, лифт едет дальше,
  // иначе, отправляется сообщение следующему желающему зайти в лифт жителю
  if (data[2 * spawn_floor + (1 + direction) / 2] == 0) {
    mes.type = 4;
    send_message(mesid, &mes, "I AM THE LAST PASSENGER");
  } else {
    send_message(mesid, &mes, "UP/DOWN");
  }
  sem_post(sem_shmem);

  // Ждем нужный этаж
  mes.type = 4 + dest_floor;
  read_message_wait(mesid, &mes);

  // Выходим из лифта
  sstr << CurTime() << getpid() << ": Leaving the lift\n";
  send_log(std::cout, sstr, str_mes, &mes_log, mesid_log);

  sleep(PASSENGER_EXIT_TIME);
  sem_wait(sem_shmem);

  // Количество желающих выйти на данном этаже уменьшилось на 1
  data[2 * MAX_FLOOR + dest_floor + 1] -= 1;

  // Если вышел последний пассажир, лифт едет дальше, 
  // иначе, отправляется сообщение следующему выходящему жителю
  if (data[2 * MAX_FLOOR + dest_floor + 1] > 0) {
    send_message(mesid, &mes, "DESTINATION REACHED");
  } else {
    mes.type = 4;
    send_message(mesid, &mes, "I AM THE LAST PASSENGER");
  }
  sem_post(sem_shmem);

  sstr << CurTime() << getpid() << ": I reached the destination\n";
  send_log(std::cout, sstr, str_mes, &mes_log, mesid_log);

  sem_close(sem_shmem);
  detach_memory_block(data);

  return 0;
}