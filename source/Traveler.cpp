#include "include/actor.h"
#include <unistd.h>
#include <iostream>

int main() {
  Actor trav;

  // Пассажир нажимает на кнопку
  trav.send_mess(1, "Pressing the button");
  std::cout << getpid() << ": Pressing the button\n";
  // Ждет прибытия лифта
  trav.read_wait(2);

  // Входит в лифт
  sleep(2);
  trav.send_mess(3, "I am inside");
  std::cout << getpid() << ": I am inside\n";
  // Ждет нужный этаж
  trav.read_wait(4);

  // Пассажир выходит из лифта
  sleep(2);
  trav.send_mess(5, "I am out");
  std::cout << getpid() << ": I am out\n";
  return 0;
}