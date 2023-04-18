#include "rtx.h"
#include "printf.h"
#include "sysproc_b.h"
/**
 * @brief: B process
 */
void b_proc(void) {
    while(1) {
        while(1) {
            MSG_BUF* msg = receive_message(NULL);
            send_message(PID_C, msg);
        }
    }
}
