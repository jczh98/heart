/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_HITABLE_H_
#define CHIHAYA_SRC_CORE_HITABLE_H_

#include "AABB.h"
#include "common.h"
#include "ray.h"

CHIHAYA_NAMESPACE_BEGIN

class Material;

inline void GetSphereUV(const Vector3f &p, float &u, float &v) {
  float phi = atan2(p.z, p.x);
  float theta = asin(p.y);
  u = 1 - (phi + M_PI) / (2 * M_PI);
  v = (theta + M_PI / 2) / M_PI;
}

struct HitRecord {
  Float u;
  Float v;
  Float t;
  Vector3f p;
  Vector3f normal;
  Material *mat_ptr;
};

class Hitable {
 public:
  virtual bool hit(const Ray &ray, Float t_min, Float t_max, HitRecord &rec) const = 0;
  virtual bool BoundingBox(float t0, float t1, AABB &box) const = 0;
};

class FlipNormals : public Hitable {
 public:
  FlipNormals(Hitable *p) : ptr(p) {}
  virtual bool hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
    if (ptr->hit(r, t_min, t_max, rec)) {
      rec.normal = -rec.normal;
      return true;
    } else {
      return false;
    }
  }
  virtual bool BoundingBox(float t0, float t1, AABB &box) const {
    return ptr->BoundingBox(t0, t1, box);
  }
  Hitable *ptr;
};

class Translate : public Hitable {
 public:
  Translate(Hitable *p, const Vector3f &displacement) : ptr(p), offset(displacement) {}
  virtual bool hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
    Ray moved_r(r.o - offset, r.d, r.time);
    if (ptr->hit(moved_r, t_min, t_max, rec)) {
      rec.p += offset;
      return true;
    } else {
      return false;
    }
  }
  virtual bool BoundingBox(float t0, float t1, AABB &box) const {
    if (ptr->BoundingBox(t0, t1, box)) {
      box = AABB(box.min() + offset, box.max() + offset);
      return true;
    } else {
      return false;
    }
  }
  Hitable *ptr;
  Vector3f offset;
};

class RotateY : public Hitable {
 public:
  RotateY(Hitable *p, float angle) : ptr(p) {
    float radians = (M_PI / 180.) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->BoundingBox(0, 1, bbox);
    Vector3f min(FLT_MAX, FLT_MAX, FLT_MAX);
    Vector3f max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          float x = i * bbox.max().x + (1 - i) * bbox.min().x;
          float y = j * bbox.max().y + (1 - j) * bbox.min().y;
          float z = k * bbox.max().z + (1 - k) * bbox.min().z;
          float newx = cos_theta * x + sin_theta * z;
          float newz = -sin_theta * x + cos_theta * z;
          Vector3f tester(newx, y, newz);
          for (int c = 0; c < 3; c++) {
            if (tester[c] > max[c]) {
              max[c] = tester[c];
            }
            if (tester[c] < min[c]) {
              min[c] = tester[c];
            }
          }
        }
      }
    }
    bbox = AABB(min, max);
  }
  virtual bool hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
    Vector3f origin = r.o;
    Vector3f direction = r.d;
    origin[0] = cos_theta * r.o[0] - sin_theta * r.o[2];
    origin[2] = sin_theta * r.o[0] + cos_theta * r.o[2];
    direction[0] = cos_theta * r.d[0] - sin_theta * r.d[2];
    direction[2] = sin_theta * r.d[0] + cos_theta * r.d[2];
    Ray rotated_r(origin, direction, r.time);
    if (ptr->hit(rotated_r, t_min, t_max, rec)) {
      Vector3f p = rec.p;
      Vector3f normal = rec.normal;
      p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
      p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
      normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
      normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
      rec.p = p;
      rec.normal = normal;
      return true;
    } else {
      return false;
    }
  }
  virtual bool BoundingBox(float t0, float t1, AABB &box) const {
    box = bbox;
    return hasbox;
  }
  Hitable *ptr;
  float sin_theta;
  float cos_theta;
  bool hasbox;
  AABB bbox;
};

CHIHAYA_NAMESPACE_END

#endif  //CHIHAYA_SRC_CORE_HITABLE_H_
