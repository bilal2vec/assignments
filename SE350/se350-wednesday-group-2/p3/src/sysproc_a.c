#include "rtx.h"
#include "printf.h"
#include "sysproc_a.h"

void serialize_int(int i, char* buf) {
    int num_digits = 0;
    int j = i;
    while (j > 0) {
        j /= 10;
        num_digits++;
    }

    if (num_digits == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    while (num_digits >= 0) {
        buf[num_digits] = i % 10 + '0';
        i /= 10;
        num_digits--;
    }
}

/**
 * @brief: A process
 */
void a_proc(void) {
    // register self with KCD
    MSG_BUF* msg = request_memory_block();
    msg->mtype = KCD_REG;
    msg->mtext[0] = '%';
    msg->mtext[1] = 'p';
    msg->mtext[2] = '\0';
    send_message(PID_KCD, msg);
    int count = 0;


    while(1) {
        // #ifdef DEBUG_0
        // if (msg->mtype == KCD_CMD) {
        //     // feel free to overwrite this but imagine that i am sad
        //     // while you do it
        //     if (count % 2) {
        //         msg = request_memory_block();
        //         msg->mtype = CRT_DISPLAY;
        //         sprintf(msg->mtext, "\033[0;91m");
        //         send_message(PID_CRT, msg);
        //     }

        //     for (int r = 0; r < 25; ++r) {
        //         msg = request_memory_block();
        //         msg->mtype = CRT_DISPLAY;
        //         for (int c = 0; c <= 65; ++c) {
        //             msg->mtext[c] = *((char *) (COOL_NUMBER + 65 * r + c));
        //         }
        //         msg->mtext[65] = '\0';
        //         send_message(PID_CRT, msg);
        //     }

        //     if (count % 2) {
        //         msg = request_memory_block();
        //         msg->mtype = CRT_DISPLAY;
        //         sprintf(msg->mtext, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        //         send_message(PID_CRT, msg);

        //         msg = request_memory_block();
        //         msg->mtype = CRT_DISPLAY;
        //         sprintf(msg->mtext, "\033[0m");
        //         send_message(PID_CRT, msg);
        //     }

        // }
        // #endif /* DEBUG_0 */

        msg = request_memory_block();
        msg->mtype = COUNT_REPORT;
        *((int*)msg->mtext) = count;

        send_message(PID_B, msg);

        ++count;

        release_processor();
    }
}
