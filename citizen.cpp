#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "include/message_queue.hpp"
#include "include/shared_memory.hpp"

int main() {
  // Получение доступа к семафору
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
  int mesid = get_message_queue_id(FILENAME);

  // Структура для обмена сообщениями
  Message mes;

  // Выбор этажа, где появился очередной житель
  // и этажа, куда этот житель направляется
  srand((unsigned)time(0));
  int spawn_floor = (rand() % 10) + 1;
  int dest_floor;
  do {
    dest_floor = (rand() % 10) + 1;
  } while (dest_floor == spawn_floor);

  // Житель нажимает на кнопку
  std::cout << getpid() << ": I want to get from floor " << spawn_floor << " to floor " << dest_floor << std::endl;
  std::cout << getpid() << ": Pressing button on the " << spawn_floor
            << " floor\n";
  mes.type = 1;
  int direction;
  if (dest_floor > spawn_floor) {
    direction = 1;
  } else {
    direction = -1;
  }

  sem_wait(sem_shmem);
  data[2 * spawn_floor + (1 + direction) / 2] += 1;
  if (spawn_floor > data[1]) {
    data[1] = spawn_floor;
  }
  if (spawn_floor < data[0] || data[0] == 0) {
    data[0] = spawn_floor;
  }
  sem_post(sem_shmem);

  send_message(mesid, &mes, "UP/DOWN");

  // Ожидание лифта
  mes.type = 2 + (1 + direction) / 2;
  read_message_wait(mesid, &mes);

  // Вход в лифт
  std::cout << getpid() << ": Entering lift\n";
  sem_wait(sem_shmem);
  data[2 * MAX_FLOOR + dest_floor + 1] += 1;
  data[2 * spawn_floor + (1 + direction) / 2] -= 1;
  sleep(PASSENGER_ENTER_TIME);

  if (direction == 1 && dest_floor > data[1]) {
    data[1] = dest_floor;
  }
  if (direction == -1 && (dest_floor < data[0] || data[0] == 0)) {
    data[0] = dest_floor;
  }

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
  std::cout << getpid() << ": Leaving the lift\n";
  sleep(PASSENGER_EXIT_TIME);
  sem_wait(sem_shmem);
  data[2 * MAX_FLOOR + dest_floor + 1] -= 1;

  if (data[2 * MAX_FLOOR + dest_floor + 1] > 0) {
    send_message(mesid, &mes, "DESTINATION REACHED");
  } else {
    mes.type = 4;
    send_message(mesid, &mes, "I AM THE LAST PASSENGER");
  }
  sem_post(sem_shmem);
  std::cout << getpid() << ": I reached the destination\n";

  sem_close(sem_shmem);
  detach_memory_block(data);

  return 0;
}