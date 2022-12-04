#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "shared_values.hpp"

static int get_shared_block(const char* filename, int size) {
  key_t key;
  // Request a key
  // The keyis, linked to a filename, so that other programs can access it.
  key = ftok(filename, 0);
  if (key == SHMEM_RESULT_ERROR) {
    return SHMEM_RESULT_ERROR;
  }
  // get shared block --- create it if it doesn't exist
  return shmget(key, size, 0644 | IPC_CREAT);
}

// attach a shared memory block
// associated with filename
// create it if it doesn't exist
int* attach_memory_block(const char* filename, int size) {
  int shared_block_id = get_shared_block(filename, size);
  int* result;
  if (shared_block_id == SHMEM_RESULT_ERROR) {
    return NULL;
  }
  // map the shared block into this process's memory I
  // and give me a pointer to it
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

#endif