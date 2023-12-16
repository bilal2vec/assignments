#define AUTOMATIC_SIGNAL uCondition bench;

#define WAITUNTIL(pred, before, after) \
  if (!(pred)) {                       \
    before;                            \
    bench.wait();                      \
  }                                    \
  after;

#define EXIT() \
  while (!bench.empty()) bench.signal();
