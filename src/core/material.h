/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#ifndef CHIHAYA_SRC_CORE_MATERIAL_H_
#define CHIHAYA_SRC_CORE_MATERIAL_H_

#include "common.h"
#include "hitable.h"
#include "texture.h"

CHIHAYA_NAMESPACE_BEGIN

Vector3f RandomInUnitSphere() {
  Vector3f p;
  do {
    p = Vector3f(drand48(), drand48(), drand48()) * 2.0 - Vector3f(1, 1, 1);
  } while (p.LengthSquared() >= 1.0);
  return p;
}

Vector3f Reflect(const Vector3f &v, const Vector3f &n) {
  return v - n * 2 * Dot(v, n);
}

bool Refract(const Vector3f &v, const Vector3f &n, float ni_over_nt, Vector3f &refracted) {
  Vector3f uv = Normalize(v);
  Float dt = Dot(uv, n);
  Float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
  if (discriminant > 0) {
    refracted = (uv - n * dt) * ni_over_nt - n * sqrt(discriminant);
    return true;
  } else
    return false;
}

Float Schlick(Float cosine, Float ref_idx) {
  Float r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 *= r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class Material {
 public:
  virtual bool Scatter(const Ray &ray, const HitRecord &rec, Vector3f &attenuation, Ray &scattered) const = 0;
  virtual Vector3f Emitted(float u, float v, const Vector3f &p) const {
    return Vector3f(0, 0, 0);
  }
};

class DiffuseLight : public Material {
 public:
  DiffuseLight(Texture *a) : emit(a) {}
  virtual bool Scatter(const Ray &r, const HitRecord &rec, Vector3f &attenuation, Ray &scattered) const {
    return false;
  }
  virtual Vector3f Emitted(float u, float v, const Vector3f &p) const {
    auto ret = emit->Value(u, v, p);
    return ret;
  }
  Texture *emit;
};

class Lambertian : public Material {
 public:
  Lambertian(Texture *a) : albedo{a} {}

  virtual bool Scatter(const Ray &ray, const HitRecord &rec, Vector3f &attenuation, Ray &scattered) const {
    Vector3f target = rec.p + rec.normal + RandomInUnitSphere();
    scattered = Ray(rec.p, target - rec.p);
    attenuation = albedo->Value(rec.u, rec.v, rec.p);
    return true;
  }

  Texture *albedo;
};

class Metal : public Material {
 public:
  Metal(const Vector3f &a, Float f) : albedo{a} {
    if (f < 1)
      fuzz = f;
    else
      fuzz = 1;
  }

  virtual bool Scatter(const Ray &ray, const HitRecord &rec, Vector3f &attenuation, Ray &scattered) const {
    Vector3f reflected = Reflect(Normalize(ray.d), rec.normal);
    scattered = Ray(rec.p, reflected + RandomInUnitSphere() * fuzz);
    attenuation = albedo;
    return (Dot(scattered.d, rec.normal) > 0);
  }

  Vector3f albedo;
  Float fuzz;
};

class Dielectric : public Material {
 public:
  Dielectric(Float ri) : ref_idx{ri} {}

  virtual bool Scatter(const Ray &ray, const HitRecord &rec, Vector3f &attenuation, Ray &scattered) const {
    Vector3f outward_normal;
    Vector3f reflected = Reflect(ray.d, rec.normal);
    Float ni_over_nt;
    attenuation = Vector3f(1.0, 1.0, 1.0);
    Vector3f refracted;
    Float reflect_prob;
    Float cosine;
    if (Dot(ray.d, rec.normal) > 0) {
      outward_normal = -rec.normal;
      ni_over_nt = ref_idx;
      cosine = ref_idx * Dot(ray.d, rec.normal) / ray.d.Length();
    } else {
      outward_normal = rec.normal;
      ni_over_nt = 1.0 / ref_idx;
      cosine = -Dot(ray.d, rec.normal) / ray.d.Length();
    }
    if (Refract(ray.d, outward_normal, ni_over_nt, refracted)) {
      reflect_prob = Schlick(cosine, ref_idx);
    } else {
      scattered = Ray(rec.p, reflected);
      reflect_prob = 1.0;
    }
    if (drand48() < reflect_prob) {
      scattered = Ray(rec.p, reflected);
    } else {
      scattered = Ray(rec.p, refracted);
    }
    return true;
  }
  Float ref_idx;
};
CHIHAYA_NAMESPACE_END

#endif  //CHIHAYA_SRC_CORE_MATERIAL_H_
