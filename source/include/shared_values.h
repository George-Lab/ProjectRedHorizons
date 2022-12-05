#ifndef SHARED_VALUES
#define SHARED_VALUES

// all of the programs will share these values
#define BLOCK_SIZE 4096
#define FILENAME "../../source/lift.cpp"
#define FILENAME_LOGGER "../../source/logger.cpp"
#define LOG_PATH "../../test/log.txt"
#define SHMEM_RESULT_ERROR (-1)

#define SEM_SHMEM_BLOCK "/shmemblock"

#define MAXBUF 100
#define MESQUEUE_RESULT_ERROR (-1)

#define MAX_FLOOR 10
#define MODE_NEUTRAL 0
#define MODE_UP 1
#define MODE_DOWN (-1)
#define SWITCH_FLOOR_TIME 2
#define PASSENGER_EXIT_TIME 2
#define PASSENGER_ENTER_TIME 2

#endif