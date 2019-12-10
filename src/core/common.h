/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_COMMON_H_
#define CHIHAYA_SRC_CORE_COMMON_H_

#define _USE_MATH_DEFINES
#include <cfloat>
#include <cmath>
#ifndef MAXFLOAT
#define MAXFLOAT FLT_MAX
#endif
#include <fmt/format.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include "glog/logging.h"
#include "random.h"

namespace chihaya {

using Float = float;

#define Infinity std::numeric_limits<Float>::infinity()

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define CHIHAYA_NAMESPACE_BEGIN namespace chihaya {
#define CHIHAYA_NAMESPACE_END }

template <typename T>
inline bool isNaN(const T x) {
  return std::isnan(x);
}
template <>
inline bool isNaN(const int x) {
  return false;
}

}  // namespace chihaya

#endif  //CHIHAYA_SRC_CORE_COMMON_H_
