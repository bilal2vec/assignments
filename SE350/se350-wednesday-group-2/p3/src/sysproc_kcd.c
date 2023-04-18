#include "rtx.h"
#include "printf.h"
#include "sysproc_kcd.h"
/**
 * @brief: KCD process for recieving keystrokes, echoing them to CRT and dispatching
 * commands
 */

void kcd_proc(void) {
    signed char cmd_char_to_registrant_pid[256];
	
    debug(("Entering kcd_proc\r\n"));
    MSG_BUF *msg;
    int sender_pid;

    // Command (char) to command handler process pid (int)
    // Initialize the handler of every possible command to -1 (no handler)
    for (int i = 0; i < 256; ++i) {
        cmd_char_to_registrant_pid[i] = -1;
    }

    char cmd_buffer[65];
    cmd_buffer[64] = '\0';
    int cmd_buffer_length = 0;
    char cmd_identifier;
    char key;
    MSG_BUF* msg_to_send;

    while(1) {
        msg = receive_message(&sender_pid);

        switch(msg->mtype) {
            case KEY_IN:
                if (sender_pid != PID_UART_IPROC) {
                    debug(("kcd_proc: FATAL ERROR: Non UART-IPROC sent KEY_IN to KCD"));
					return;
                }
                key = msg->mtext[0];

                // Echo the key to CRT
                msg_to_send = msg;
                msg_to_send->mtype = CRT_DISPLAY;
                send_message(PID_CRT, msg_to_send);

                // Command entry complete (enter key pressed)
                if (key == '\r') {
                    // Determine command identifier
                    cmd_identifier = cmd_buffer[1];
					
                    // Try to send command
                    msg_to_send = request_memory_block();

                    int recipient_pid = cmd_char_to_registrant_pid[cmd_identifier % 256];

                    if (cmd_buffer[0] != '%' || cmd_buffer_length > 64) {
                        // Command is too long or doesn't start with %
                        msg_to_send->mtype = CRT_DISPLAY;
						
                        sprintf(msg_to_send->mtext, "Invalid command\r");
                        send_message(PID_CRT, msg_to_send);
                    } else if (recipient_pid < 0) {
                        // Command identifier doesn't have registered process
                        msg_to_send->mtype = CRT_DISPLAY;
						
                        sprintf(msg_to_send->mtext, "Command not found\r");
                        send_message(PID_CRT, msg_to_send);
                    } else {
                        // Copy command to cmd_msg mtext
						cmd_buffer[cmd_buffer_length % 64] = '\0';
                        sprintf(msg_to_send->mtext, "%s", cmd_buffer);
						
                        msg_to_send->mtype = KCD_CMD;
                        send_message(recipient_pid, msg_to_send);
                    }
					// clear the buffer
                    cmd_buffer_length = 0;
					cmd_buffer[0] = '\0';
                } else {
                    cmd_buffer[cmd_buffer_length % 64] = key;
                    ++cmd_buffer_length;
                }
                break;
            case KCD_REG:
                if (msg->mtext[0] != '%' || msg->mtext[2] != '\0') {
                    debug(("kcd_proc: FATAL ERROR: malformed KCD_REG MSG: %s \r\n",  msg->mtext));
                }
                cmd_identifier = msg->mtext[1];
                cmd_char_to_registrant_pid[cmd_identifier] = sender_pid;
                release_memory_block(msg);
                break;
            default:
                debug(("kcd_proc: FATAL ERROR: received unexpected msg type"));
                release_memory_block(msg);
                break;
        }

        release_processor();
    }
}
