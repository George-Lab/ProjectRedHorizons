#include "actor.h"

#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "message_queue.h"

Actor::Actor() {
  // Создание очереди сообщений
  mesid = get_message_queue_id("Lift_starter.out");
}

void Actor::read_wait(int mess_type) {
  read_message_wait(mesid, &Mess, mess_type);
}

void Actor::read_nowait(int mess_type) {
  read_message_nowait(mesid, &Mess, mess_type);
}

void Actor::send_mess(int mess_type, const char* text) {
  send_message(mesid, &Mess, text, mess_type);
}