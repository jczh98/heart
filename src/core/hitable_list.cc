/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#include "hitable_list.h"

CHIHAYA_NAMESPACE_BEGIN

bool HitableList::hit(const chihaya::Ray &ray,
                      chihaya::Float t_min,
                      chihaya::Float t_max,
                      chihaya::HitRecord &rec) const {
  HitRecord temp_rec;
  bool hit = false;
  Float closest = t_max;
  for (int i = 0; i < list_size; ++i) {
    if (list[i]->hit(ray, t_min, closest, temp_rec)) {
      hit = true;
      closest = temp_rec.t;
      rec = temp_rec;
    }
  }
  return hit;
}

bool HitableList::BoundingBox(float t0, float t1, AABB &box) const {
  if (list_size < 1) return false;
  AABB temp;
  bool first = list[0]->BoundingBox(t0, t1, box);
  if (!first) {
    return false;
  } else {
    box = temp;
  }
  for (int i = 1; i < list_size; i++) {
    if (list[i]->BoundingBox(t0, t1, temp)) {
      box = SurroundingBox(box, temp);
    } else {
      return false;
    }
  }
  return true;
}

CHIHAYA_NAMESPACE_END
