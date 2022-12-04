#include <unistd.h>

#include <cstdlib>
#include <ctime>

int main() {
  srand((unsigned)time(0));
  int random_sleep_time;
  
  while (true) {
    random_sleep_time = (rand() & 30) + 1;

    if (fork() == 0) {
      execl("citizen.out", "citizen.out", NULL);
    }
    
    sleep(random_sleep_time);
  }

  return 0;
}