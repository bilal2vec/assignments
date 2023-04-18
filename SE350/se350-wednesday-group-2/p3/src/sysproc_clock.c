#include "rtx.h"
#include "printf.h"
#include "sysproc_clock.h"
/**
 * @brief: Clock process
 */

void increment_one_second(unsigned char* hour, unsigned char* minute, unsigned char* second) {
    // Increment the provided time by one second in-place
    (*second)++;
    if (*second == 60) {
        *second = 0;
        (*minute)++;
    }
    if (*minute == 60) {
        *minute = 0;
        (*hour)++;
    }
    if (*hour == 24) {
        *hour = 0;
    }
}

void send_time_to_crt(unsigned char hour, unsigned char minute, unsigned char second) {
    MSG_BUF* to_crt = request_memory_block();
    to_crt->mtype = CRT_DISPLAY;

    sprintf(to_crt->mtext, "%02d:%02d:%02d\r", hour, minute, second);

    send_message(PID_CRT, to_crt);
}

// time_str is a two digit string e.g. "00", "15", base is the number at which
// the time wraps around e.g. base=60 for seconds/minutes, base=24 for hours
// Provide base so that if time_str gives something out of bounds we can
// wrap it into bounds (though this is invalid input)
unsigned char parse_two_digits_to_time(char *time_str, unsigned char base) {
    // Extract the tens/ones digit
    unsigned char tens_place = time_str[0] - '0';
    unsigned char ones_place = time_str[1] - '0';

    if (ones_place > 9) {
        ones_place = 0;
    }

    if (tens_place > 9) {
        tens_place = 0;
    }

    // Compute time
    unsigned char result = (tens_place % base) * 10 + ones_place % base;

    // in case the chars were invalid just set to 0
    if (result >= base) {
        result = 0;
    }

    return result;
}

void clock_proc(void) {
    // Register self with KCD
    MSG_BUF* msg = request_memory_block();
    msg->mtype = KCD_REG;
    msg->mtext[0] = '%';
    msg->mtext[1] = 'W';
    msg->mtext[2] = '\0';
    send_message(PID_KCD, msg);

    // Initialize clock state
    BOOL clock_active = FALSE;
    unsigned char hour = 0;
    unsigned char minute = 0;
    unsigned char second = 0;

    // Send initial timing message to self
    MSG_BUF* timing_msg = request_memory_block();
    timing_msg->mtype = TIMING_1S;
    timing_msg->mtext[0] = '\0';
    delayed_send(PID_CLOCK, timing_msg, 1000);

    while(1) {
        msg = receive_message(NULL);

        // the CIA wants you to believe switch statements
        // get compiled into if/else, but this is not true
        switch(msg->mtype) {
            case TIMING_1S:
                // update clock
                increment_one_second(&hour, &minute, &second);

                // if clock is active, send a formatted time string to CRT
                if (clock_active) {
                    send_time_to_crt(hour, minute, second);
                }

                // reuse the same message for timing
                delayed_send(PID_CLOCK, msg, 1000);
                break;
            case KCD_CMD:
                // command format is %W[char] [arg]
                switch(msg->mtext[2]) {
                    case 'R':
                        hour = 0;
                        minute = 0;
                        second = 0;

                        clock_active = TRUE;
                        break;
                    case 'S':
                        //%WS 12:23:34
                        hour = parse_two_digits_to_time(&msg->mtext[4], 24);
                        minute = parse_two_digits_to_time(&msg->mtext[7], 60);
                        second = parse_two_digits_to_time(&msg->mtext[10], 60);

                        clock_active = TRUE;
                        send_time_to_crt(hour, minute, second);
                        break;
                    case 'T':
                        clock_active = FALSE;
                        break;
                    default:
                        break;
                }
                break;
            default:
                debug(("clock_proc: FATAL ERROR: Unexpected message type in clock_proc\r\n"));
                release_memory_block(msg);
                break;
        }
        release_processor();
    }
}
