#ifndef _state_h_
#define _state_h_

typedef enum State {
  ACTIVE = 0,
  READY,
  EXITED,
  BLOCKED_ON_SEND,
  BLOCKED_ON_RECEIVE,
  BLOCKED_ON_REPLY,
  BLOCKED_ON_EVENT
} State;

#endif /* state.h */