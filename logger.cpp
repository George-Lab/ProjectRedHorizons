#include <fcntl.h>
#include <semaphore.h>

#include <fstream>
#include <iostream>

#include "include/shared_memory.hpp"

int main() {
  sem_unlink(SEM_PRODUCER_FILENAME);
  sem_unlink(SEM_CONSUMER_FILENAME);

  sem_t* sem_prod = sem_open(SEM_SHMEM_BLOCK, O_CREAT, 0660, 0);
  if (sem_prod == SEM_FAILED) {
    perror("sem_open/sem_shmem");
    exit(EXIT_FAILURE);
  }
  sem_t* sem_cons = sem_open(SEM_SHMEM_BLOCK, O_CREAT, 0660, 1);
  if (sem_cons == SEM_FAILED) {
    perror("sem_open/sem_shmem");
    exit(EXIT_FAILURE);
  }

  char* log_block = attach_memory_block<char>(FILENAME_LOGGER, BLOCK_SIZE);
    if (log_block == NULL) {
    std::cout << "ERROR: couldn't get block\n";
    return -1;
  } 

  return 0;
}