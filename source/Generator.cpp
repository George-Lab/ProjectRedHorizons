#include <unistd.h>

#include <cstdlib>
#include <ctime>

int main() {
  srand((unsigned)time(0));
  int random_sleep_time;
  
  while (true) {
    random_sleep_time = (rand() & 10) + 1;

    if (fork() == 0) {
      execl("Traveler.out", "Traveler.out", NULL);
    }
    // if (fork() == 0) {
    //   execl("Traveler.out", "Traveler.out", NULL);
    // }
    
    sleep(random_sleep_time);
  }

  return 0;
}