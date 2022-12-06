#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "shared_values.h"

static int get_shared_block(const char* filename, int size) {
  key_t key;
  // Создание ключа
  // Ключ связан с файлом для того, чтобы другие процессы
  // могли получить доступ
  key = ftok(filename, 0);
  if (key == SHMEM_RESULT_ERROR) {
    return SHMEM_RESULT_ERROR;
  }
  // Получение блока разделяемой памяти
  // Если блока не существует, тогда он создается
  return shmget(key, size, 0644 | IPC_CREAT);
}

// Подключить блок разделяемой памяти,
// связанный с файлом
// Создать блок, если он не существует
int* attach_memory_block(const char* filename, int size) {
  int shared_block_id = get_shared_block(filename, size);
  int* result;
  if (shared_block_id == SHMEM_RESULT_ERROR) {
    return NULL;
  }

  // Подключить блок памяти к адрессному пространству
  // процесса. Вернуть указатель на блок
  result = (int*)shmat(shared_block_id, NULL, 0);
  if (result == (int*)SHMEM_RESULT_ERROR) {
    return NULL;
  }

  return result;
}

bool detach_memory_block(int* block) {
  return (shmdt(block) != SHMEM_RESULT_ERROR);
}

bool destroy_memory_block(const char* filename) {
  int shared_block_id = get_shared_block(filename, 0);

  if (shared_block_id == SHMEM_RESULT_ERROR) {
    return NULL;
  }
  return (shmctl(shared_block_id, IPC_RMID, NULL) != SHMEM_RESULT_ERROR);
}