#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#include <iostream>

#include "include/message_queue.h"
#include "include/shared_memory.h"

int main() {
  // Создание семафора
  sem_unlink(SEM_SHMEM_BLOCK);

  sem_t* sem_shmem = sem_open(SEM_SHMEM_BLOCK, O_CREAT, 0660, 1);
  if (sem_shmem == SEM_FAILED) {
    perror("sem_open/sem_shmem");
    exit(EXIT_FAILURE);
  }

  // Создание области разделяемой памяти
  int* data = attach_memory_block(FILENAME, BLOCK_SIZE);
  if (data == NULL) {
    std::cout << "ERROR: couldn't get block\n";
    return -1;
  }

  // Подготовка памяти
  for (int i = 0; i < 3 * MAX_FLOOR + 2; ++i) {
    data[i] = 0;
  }

  // Создание очереди сообщений
  int mesid = create_message_queue(FILENAME);
  if (mesid < 0) {
    printf("Can't create message's queue\n");
    exit(1);
  }

  // Создание очереди сообщений для logger'а
  int mesid_log = create_message_queue(FILENAME_LOGGER);
  if (mesid_log < 0) {
    printf("Can't create message's queue\n");
    exit(1);
  }

  // Структуры для обмена сообщениями
  Message mes; // Для общения с citizen
  Message mes_log; // Для общения с logger
  mes_log.type = 1;

  int cur_floor = 1;
  int working_direction = MODE_NEUTRAL;

  while (true) {
    if (working_direction == MODE_NEUTRAL) {
      // Ожидание первого вызова
      mes.type = 1;
      read_message_wait(mesid, &mes);
      // Считывание всех поступивших вызовов
      while (true) {
        if (read_message_nowait(mesid, &mes) == -1) {
          break;
        }
      }

      // Определяем направление движения лифта.
      // Приоритет - движение вверх
      sem_wait(sem_shmem);
      if (data[1] < cur_floor) {
        working_direction = MODE_DOWN;
      } else {
        working_direction = MODE_UP;
      }
      sem_post(sem_shmem);
    }

    bool doors_opened = false;
    // Проверяем, нужно ли высаживать кого-то на текущем этаже
    sem_wait(sem_shmem);
    if (data[2 * MAX_FLOOR + 1 + cur_floor] > 0) {
      sem_post(sem_shmem);
      // Сообщаем пассажирам, что лифт прибыл
      std::cout << "Lift is opening the doors on the " << cur_floor << " floor\n";
      doors_opened = true;
      mes.type = 4 + cur_floor;
      send_message(mesid, &mes, "DESTINATION REACHED");

      mes.type = 4;
      read_message_wait(mesid, &mes);
      sem_wait(sem_shmem);
    }

    // Проверяем, хочет ли кто-нибудь зайти в лифт на текущем этаже
    // и поехать в текущем направлении движении лифта
    if (data[2 * cur_floor + (1 + working_direction) / 2] > 0) {
      mes.type = 2 + (1 + working_direction) / 2;
      send_message(mesid, &mes, "I AM HERE!");
      if (!doors_opened) { 
        std::cout << "Lift is opening the doors on the " << cur_floor << " floor\n";
        doors_opened = true;
      }

      // Считываем нажатие кнопок пассажирами
      sem_post(sem_shmem);
      while (true) {
        mes.type = 4;
        read_message_wait(mesid, &mes);
        if (strcmp(mes.buf, "I AM THE LAST PASSENGER") == 0) {
          break;
        }
      }
      sem_wait(sem_shmem);
    }
    if (doors_opened) {
      std::cout << "Lift is closing the doors on the " << cur_floor << " floor\n";
    }

    // Проверяем, нужно ли ехать дальше в текущем направлении
    if (data[(1 + working_direction) / 2] == cur_floor ||
        data[(1 + working_direction) / 2] == 0) {
      data[(1 + working_direction) / 2] = 0;

      // Если текущих вызовов нет, то переходим в нейтральный режим
      if (data[0] == 0 && data[1] == 0) {
        working_direction = MODE_NEUTRAL;
        sem_post(sem_shmem);
        continue;
      }

      // Меняем направление движения на противоположное, но остаемся на
      // текущем этаже
      working_direction = 0 - working_direction;
      sem_post(sem_shmem);
      continue;
    } else if (data[((1 + working_direction) / 2 + 1) % 2] == cur_floor &&
               data[2 * cur_floor + ((1 + working_direction) / 2 + 1) % 2] ==
                   0) {
      data[((1 + working_direction) / 2 + 1) % 2] += working_direction;
    }
    sem_post(sem_shmem);

    // Едем на следующий этаж
    sleep(SWITCH_FLOOR_TIME);
    cur_floor += working_direction;
    std::cout << "going to the next floor: " << cur_floor << std::endl;
  }

  sem_close(sem_shmem);
  detach_memory_block(data);

  return 0;
}