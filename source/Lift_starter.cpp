#include <unistd.h>

#include <iostream>

#include "include/lift.h"

int main() {
  Lift Elev;

  while (true) {
    // Ждем, пока кто-нибудь нажмет кнопку
    Elev.read_wait(1);
    Elev.pass_pressed_button();
    Elev.Log("Button pressed\n");
    std::cout << "Button pressed\n";

    // Считываем все нажатия кнопки
    while (true) {
      if (Elev.read_nowait(1) == -1) {
        break;
      }
      Elev.pass_pressed_button();
      Elev.Log("Button pressed\n");
      std::cout << "Button pressed\n";
    }

    // Поочередно запускаем пассажирова, пока
    // все не зайдут
    while (!Elev.NobodyWaitingOutside()) {
      Elev.send_mess(2, "You can enter the lift");
      Elev.Log("Passenger is entering the lift\n");
      std::cout << "Passenger is entering the lift\n";

      // Ждем пассажира
      Elev.read_wait(3);
      Elev.pass_entered();

      // Проверяем, нажал ли кто-то еще кнопку
      while (true) {
        if (Elev.read_nowait(1) == -1) {
          break;
        }
        Elev.pass_pressed_button();
        Elev.Log("Button pressed\n");
        std::cout << "Button pressed\n";
      }
    }

    // Едем на следующий этаж
    Elev.Log("Lift is going to the 2nd floor\n");
    std::cout << "Lift is going to the 2nd floor\n";
    sleep(2);

    // Поочередно выпускаем пассажиров, пока все
    // не выйдут
    while (!Elev.NobodyWaitingInside()) {
      Elev.send_mess(4, "Destination reached");
      Elev.Log("Passenger is leaving the lift\n");
      std::cout << "Passenger is leaving the lift\n";

      // Ждем пассажира
      Elev.read_wait(5);
      Elev.pass_exited();
    }

    // Возвращаемся на первый этаж
    Elev.Log("Lift is going to the first floor\n");
    Elev.UpdateLog();
    std::cout << "Lift is going to the first floor\n";
    sleep(2);
  }

  return 0;
}