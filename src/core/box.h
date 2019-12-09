/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_BOX_H_
#define CHIHAYA_SRC_CORE_BOX_H_

#include "hitable_list.h"
#include "rect.h"

CHIHAYA_NAMESPACE_BEGIN

class Box : public Hitable {
 public:
  Box() {}
  Box(const Vector3f& p0, const Vector3f& p1, Material* ptr) {
    pmin = p0;
    pmax = p1;
    Hitable** list = new Hitable*[6];
    list[0] = new XYrect(p0.x, p1.x, p0.y, p1.y, p1.z, ptr);
    list[1] = new FlipNormals(new XYrect(p0.x, p1.x, p0.y, p1.y, p0.z, ptr));
    list[2] = new XZrect(p0.x, p1.x, p0.z, p1.z, p1.y, ptr);
    list[3] = new FlipNormals(new XZrect(p0.x, p1.x, p0.z, p1.z, p0.y, ptr));
    list[4] = new YZrect(p0.y, p1.y, p0.z, p1.z, p1.x, ptr);
    list[5] = new FlipNormals(new YZrect(p0.y, p1.y, p0.z, p1.z, p0.x, ptr));
    list_ptr = new HitableList(list, 6);
  }
  virtual bool hit(const Ray& r, Float t0, Float t1, HitRecord& rec) const {
    return list_ptr->hit(r, t0, t1, rec);
  }
  virtual bool BoundingBox(float t0, float t1, AABB& box) const {
    box = AABB(pmin, pmax);
    return true;
  }
  Vector3f pmin, pmax;
  Hitable* list_ptr;
};

CHIHAYA_NAMESPACE_END

#endif