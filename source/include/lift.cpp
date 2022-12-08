#include "lift.h"

#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <iostream>

#include "message_queue.h"

Lift::Lift() {
  // Создание очереди сообщений
  mesid = create_message_queue("Lift_starter.out");

  waiting = 0;
  inside = 0;

  logfile.open("../log/log.txt");
  if (!logfile) {
    std::cerr << "Couldn't open the file\n";
    exit(1);
  }
}

Lift::~Lift() {
  delete_message_queue(mesid);
  logfile.close();
}

int Lift::read_wait(int mess_type) {
  read_message_wait(mesid, &Mess, mess_type);

  return 1;
}

int Lift::read_nowait(int mess_type) {
  return read_message_nowait(mesid, &Mess, mess_type);
}

void Lift::send_mess(int mess_type, const char* text) {
  send_message(mesid, &Mess, text, mess_type);
}

void Lift::Log(std::string s) { 
  logfile << s; 
  // std::cout << waiting << std::endl;
  }

void Lift::UpdateLog() { logfile.flush(); }

void Lift::pass_pressed_button() { ++waiting; }

void Lift::pass_entered() {
  --waiting;
  ++inside;
}

void Lift::pass_exited() {
  --inside;
}

bool Lift::NobodyWaitingOutside() { return waiting == 0; }

bool Lift::NobodyWaitingInside() { return inside == 0; }