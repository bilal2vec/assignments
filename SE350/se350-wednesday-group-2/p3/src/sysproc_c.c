#include "sysproc_c.h"
#include "circ_buf.h"
#include "printf.h"
#include "queue.h"
#include "rtx.h"
#include <stdint.h>

CIRC_BUF local_message_queue;

int
deserialize_int(char* buf)
{
  int i = 0;
  while (*buf != '\0') {
    i *= 10;
    i += *buf - '0';
    buf++;
  }
  return i;
}

MSG_BUF*
hibernate()
{
  MSG_BUF* msg = request_memory_block();
  msg->mtype = WAKEUP_10;
  delayed_send(PID_C, msg, 10000);

  while (1) {
    msg = receive_message(NULL);
    if (msg->mtype == WAKEUP_10) {
      return msg;
    } else {
      int rc = cb_push_back(&local_message_queue, msg);
      if (rc != RTX_OK) {
        debug(("hibernate: FATAL ERROR: enqueue failed\r"));
      }
    }
  }
}

/**
 * @brief: C process
 */
void
c_proc(void)
{
  cb_init(&local_message_queue);
  while (1) {
    MSG_BUF* msg;
    if (cb_is_empty(&local_message_queue)) {
      msg = receive_message(NULL);
    } else {
      msg = cb_pop_front(&local_message_queue);
    }

    if (msg->mtype == COUNT_REPORT) {
      int count = *((int*)msg->mtext);

      if (count % 20 == 0) {
        sprintf(msg->mtext, "Process C\r");
        msg->mtype = CRT_DISPLAY;
        send_message(PID_CRT, msg);
        msg = hibernate();
      }
    }

    release_memory_block(msg);
    release_processor();
  }
}
