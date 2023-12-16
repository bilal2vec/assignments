#include <iostream>

#include <uCobegin.h>
#include <uActor.h>

using namespace std;

template<typename T> void quicksort( T values[], int low, int high, unsigned int depth );

// Task definition
#ifdef TASK
_Task QuickSort {
  STYPE* values;
  int low;
  int high;
  unsigned int depth;
  void main() {
    quicksort(values, low, high, depth);
  }
public:
  QuickSort(STYPE* values, int low, int high, unsigned int depth) : values(values), low(low), high(high), depth(depth) {}
};
#endif

// Actor definition
#ifdef ACTOR
struct QuickSortMsg : public uActor::Message {
  STYPE* values;
  int low;
  int high;
  unsigned int depth;
  QuickSortMsg(STYPE* values, int low, int high, unsigned int depth) : uActor::Message(uActor::Delete), values(values), low(low), high(high), depth(depth) {}
};
_Actor QuickSort {
  uActor::Allocation receive(uActor::Message& msg) {
    Case(QuickSortMsg, msg) {
      quicksort(msg_d->values, msg_d->low, msg_d->high, msg_d->depth);
    } else Case(uActor::StopMsg, msg) {
      return uActor::Delete;
    }
    return uActor::Nodelete;
  }
};
#endif


template<typename T>
void quicksort( T values[], int low, int high, unsigned int depth ){
  if (low < high) {

    // find pivot
    T pivot = values[low + (high - low) / 2];

    // Partition sequentially
    int i = low - 1;
    int j = high + 1;

    while (true) {
      while (values[++i] < pivot);
      while (values[--j] > pivot);

      if (i >= j) {
        break;
      }

      swap(values[i], values[j]);
    }

    int pivot_idx = j;

    // Recurse in parallel
    if (depth > 0) {

      #ifdef CBEGIN
      COBEGIN
      BEGIN quicksort(values, low, pivot_idx, depth - 1); END
      BEGIN quicksort(values, pivot_idx + 1, high, depth - 1); END
      COEND
      #endif
      
      #ifdef TASK
      {
        QuickSort left(values, low, pivot_idx, depth - 1);
        QuickSort right(values, pivot_idx + 1, high, depth - 1);
      }
      #endif

      #ifdef ACTOR
      {
        *new QuickSort() | *new QuickSortMsg(values, low, pivot_idx, depth - 1) | uActor::stopMsg;
        *new QuickSort() | *new QuickSortMsg(values, pivot_idx + 1, high, depth - 1) | uActor::stopMsg;
      }
      #endif

    // Recurse sequentially
    } else {
      quicksort(values, low, pivot_idx, depth);
      quicksort(values, pivot_idx + 1, high, depth);
    }
  }
}
