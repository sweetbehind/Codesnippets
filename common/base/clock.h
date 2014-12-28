
#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

// 从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
class MonotonicClock {
 public:
  static int64_t MicroSeconds();
  static int64_t MilliSeconds();
};

// 系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,
// 中间时刻如果系统时间被用户改成其他,则对应的时间相应改变
class RealtimeClock {
 public:
  static int64_t MicroSeconds();
  static int64_t MilliSeconds();
};

#endif  // CLOCK_H_
