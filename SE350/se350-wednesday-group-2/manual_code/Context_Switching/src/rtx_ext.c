#include "k_rtx.h"

void set_privileged() {
	U32 ctrl = __get_CONTROL();
  ctrl &= ~BIT(0);    // set bit(0) to privileged
  __set_CONTROL(ctrl);
}

void set_unprivileged() {
	U32 ctrl = __get_CONTROL();
  ctrl |= BIT(0);    // set bit(0) to unprivileged
  __set_CONTROL(ctrl);
}