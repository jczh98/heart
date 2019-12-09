/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */
#ifndef CHIHAYA_SRC_CORE_RECT_H_
#define CHIHAYA_SRC_CORE_RECT_H_

#include "hitable.h"

CHIHAYA_NAMESPACE_BEGIN

class XYrect : public Hitable {
 public:
  XYrect() {}
  XYrect(float _x0, float _x1, float _y0, float _y1, float _k, Material* mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat){};
  virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.o.z) / r.d.z;
    if (t < t0 || t > t1)
      return false;
    float x = r.o.x + t * r.d.x;
    float y = r.o.y + t * r.d.y;
    if (x < x0 || x > x1 || y < y0 || y > y1)
      return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r(t);
    rec.normal = Vector3f(0, 0, 1);
    return true;
  }

  virtual bool BoundingBox(float t0, float t1, AABB& box) const {
    box = AABB(Vector3f(x0, y0, k - 0.0001), Vector3f(x1, y1, k + 0.0001));
    return true;
  }
  Material* mp;
  float x0, x1, y0, y1, k;
};

class XZrect : public Hitable {
 public:
  XZrect() {}
  XZrect(float _x0, float _x1, float _z0, float _z1, float _k, Material* mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {}
  virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.o.y) / r.d.y;
    if (t < t0 || t > t1)
      return false;
    float x = r.o.x + t * r.d.x;
    float z = r.o.z + t * r.d.z;
    if (x < x0 || x > x1 || z < z0 || z > z1)
      return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r(t);
    rec.normal = Vector3f(0, 1, 0);
    return true;
  }
  virtual bool BoundingBox(float t0, float t1, AABB& box) const {
    box = AABB(Vector3f(x0, k - 0.0001, z0), Vector3f(x1, k + 0.0001, z1));
    return true;
  }
  Material* mp;
  float x0, x1, z0, z1, k;
};

class YZrect : public Hitable {
 public:
  YZrect() {}
  YZrect(float _y0, float _y1, float _z0, float _z1, float _k, Material* mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {}
  virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.o.x) / r.d.x;
    if (t < t0 || t > t1)
      return false;
    float y = r.o.y + t * r.d.y;
    float z = r.o.z + t * r.d.z;
    if (y < y0 || y > y1 || z < z0 || z > z1)
      return false;
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r(t);
    rec.normal = Vector3f(1, 0, 0);
    return true;
  }
  virtual bool BoundingBox(float t0, float t1, AABB& box) const {
    box = AABB(Vector3f(k - 0.0001, y0, z0), Vector3f(k + 0.0001, y1, z1));
    return true;
  }
  Material* mp;
  float y0, y1, z0, z1, k;
};

CHIHAYA_NAMESPACE_END

#endif
