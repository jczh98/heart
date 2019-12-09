#ifndef CHIHAYA_SRC_CORE_RANDOM_H_
#define CHIHAYA_SRC_CORE_RANDOM_H_

#include <cstdlib>

#if defined(_MSC_VER)
inline double drand48() {
  return rand() / (RAND_MAX + 1.0);
}
#endif

#endif