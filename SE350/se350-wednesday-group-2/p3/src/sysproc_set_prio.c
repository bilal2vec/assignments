#include "k_msg.h"
#include "rtx.h"
#include "sysproc_set_prio.h"

void deliver_error_to_crt() {
    MSG_BUF* to_crt = request_memory_block();
    to_crt->mtype = CRT_DISPLAY;
    // crazy hack to avoid using strcpy or writing annoying code
    sprintf(to_crt->mtext, "Invalid command input\r");

    send_message(PID_CRT, to_crt);
}

// negative or zero output means something was invalid
signed char parse_digits_till_space(signed char* result, char* str) {
    // Initialize variables to keep track of the parsed integer and its place value
    *result = 0;
    unsigned char place_value = 1;

    int i = 0;

    // skip leading spaces
    while (str[i] == ' ') {
        ++i;
    }

    // iterate until we reach a space
    while (str[i] != '\0' && str[i] != ' ') {
        // check if the current character is a digit
        if (str[i] >= '0' && str[i] <= '9') {
            // Add digit to parsed result so far
            unsigned char digitValue = str[i] - '0';
            *result = *result * 10 + digitValue;

            // Update the place value
            place_value *= 10;
        } else {
            // an invalid input was entered
            return -1;
        }
        ++i;
    }

    // Return the number of bytes parsed
    return i;
}

/**
 * @brief: set_prio process to set new priority for process
 */
void set_prio_proc(void) {
    // register self with KCD
    MSG_BUF* msg = request_memory_block();
    msg->mtype = KCD_REG;
    msg->mtext[0] = '%';
    msg->mtext[1] = 'C';
    msg->mtext[2] = '\0';
    send_message(PID_KCD, msg);

    signed char pid = -1;
    signed char new_prio = -1;

    int offset_between_args = 0;

    while(1) {
        msg = receive_message(NULL);

        // the CIA wants you to believe switch statements
        // get compiled into if/else, but this is not true
        switch(msg->mtype) {
            case KCD_CMD:
                // %C [PID] [NEW_PRIORITY]
                offset_between_args = parse_digits_till_space(&pid, &msg->mtext[3]);

                if (offset_between_args <= 0 || pid >= NUM_TOTAL_USERSPACE_PROCS + NUM_I_PROCS || pid < 0) {
                    release_memory_block(msg);
                    deliver_error_to_crt();
                } else {
                    offset_between_args = parse_digits_till_space(&new_prio, &msg->mtext[3 + offset_between_args]);
                    release_memory_block(msg);
                    if (offset_between_args <= 0 || new_prio >= NUM_PRIORITIES || new_prio < 0) {
                        deliver_error_to_crt();
                    } else {
                        set_process_priority(pid, new_prio);
                    }
                }

                break;
            default:
                release_memory_block(msg);
                break;
        }
        release_processor();
    }
}
