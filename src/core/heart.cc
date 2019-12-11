/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#include "heart.h"
#include <chrono>

CHIHAYA_NAMESPACE_BEGIN

const double eps = 1e-12;
const double inf = 1e12;

std::vector<double> PrepareCoef(Vector3f o, Vector3f d, float rr, int n) {
  std::vector<double> coef(n + 1);
  // Sphere
  //coef[0] = Dot(o, o) / (r * r) - 1;
  //coef[1] = 2 * Dot(o, d) / (r * r);
  //coef[2] = Dot(d, d) / (r * r);
  // Heart surface
  double ox = o.x, ox2 = std::pow(o.x, 2), ox3 = std::pow(o.x, 3), ox4 = std::pow(o.x, 4), ox5 = std::pow(o.x, 5), ox6 = std::pow(o.x, 6);
  double oy = o.x, oy2 = std::pow(o.y, 2), oy3 = std::pow(o.y, 3), oy4 = std::pow(o.y, 4), oy5 = std::pow(o.y, 5), oy6 = std::pow(o.y, 6);
  double oz = o.z, oz2 = std::pow(o.z, 2), oz3 = std::pow(o.z, 3), oz4 = std::pow(o.z, 4), oz5 = std::pow(o.z, 5), oz6 = std::pow(o.z, 6);
  double dx = o.x, dx2 = std::pow(d.x, 2), dx3 = std::pow(d.x, 3), dx4 = std::pow(d.x, 4), dx5 = std::pow(d.x, 5), dx6 = std::pow(d.x, 6);
  double dy = o.x, dy2 = std::pow(d.y, 2), dy3 = std::pow(d.y, 3), dy4 = std::pow(d.y, 4), dy5 = std::pow(d.y, 5), dy6 = std::pow(d.y, 6);
  double dz = o.z, dz2 = std::pow(d.z, 2), dz3 = std::pow(d.z, 3), dz4 = std::pow(d.z, 4), dz5 = std::pow(d.z, 5), dz6 = std::pow(d.z, 6);
  double r = rr, r2 = pow(r, 2), r3 = pow(r, 3), r4 = pow(r, 4), r5 = pow(r, 5), r6 = pow(r, 6);
  coef[0] = ox6 + (27 * ox4 * oy2) / 4 + (243 * ox2 * oy4) / 16 + (729 * oy6) / 64 + 3 * ox4 * oz2 +
            (27 / 2) * ox2 * oy2 * oz2 + (243 * oy4 * oz2) / 16 + 3 * ox2 * oz4 +
            (27 * oy2 * oz4) / 4 + oz6 - ox2 * oz3 * r - (9 / 80) * oy2 * oz3 * r - 3 * ox4 * r2 -
            (27 / 2) * ox2 * oy2 * r2 - (243 * oy4 * r2) / 16 - 6 * ox2 * oz2 * r2 -
            (27 / 2) * oy2 * oz2 * r2 - 3 * oz4 * r2 + 3 * ox2 * r4 + (27 * oy2 * r4) / 4 +
            3 * oz2 * r4 - r6;
  coef[1] = 6 * dx * ox5 + (27 / 2) * dy * ox4 * oy + 27 * dx * ox3 * oy2 +
            (243 / 4) * dy * ox2 * oy3 + (243 / 8) * dx * ox * oy4 + (2187 * dy * oy5) / 32 +
            6 * dz * ox4 * oz + 27 * dz * ox2 * oy2 * oz + (243 / 8) * dz * oy4 * oz +
            12 * dx * ox3 * oz2 + 27 * dy * ox2 * oy * oz2 + 27 * dx * ox * oy2 * oz2 +
            (243 / 4) * dy * oy3 * oz2 + 12 * dz * ox2 * oz3 + 27 * dz * oy2 * oz3 + 6 * dx * ox * oz4 +
            (27 / 2) * dy * oy * oz4 + 6 * dz * oz5 - 3 * dz * ox2 * oz2 * r -
            (27 / 80) * dz * oy2 * oz2 * r - 2 * dx * ox * oz3 * r - (9 / 40) * dy * oy * oz3 * r -
            12 * dx * ox3 * r2 - 27 * dy * ox2 * oy * r2 - 27 * dx * ox * oy2 * r2 -
            (243 / 4) * dy * oy3 * r2 - 12 * dz * ox2 * oz * r2 - 27 * dz * oy2 * oz * r2 -
            12 * dx * ox * oz2 * r2 - 27 * dy * oy * oz2 * r2 - 12 * dz * oz3 * r2 + 6 * dx * ox * r4 +
            (27 / 2) * dy * oy * r4 + 6 * dz * oz * r4;
  coef[2] = 15 * dx2 * ox4 + (27 * dy2 * ox4) / 4 + 3 * dz2 * ox4 + 54 * dx * dy * ox3 * oy +
            (81 / 2) * dx2 * ox2 * oy2 + (729 / 8) * dy2 * ox2 * oy2 + (27 / 2) * dz2 * ox2 * oy2 +
            (243 / 2) * dx * dy * ox * oy3 + (243 * dx2 * oy4) / 16 + (10935 * dy2 * oy4) / 64 +
            (243 * dz2 * oy4) / 16 + 24 * dx * dz * ox3 * oz + 54 * dy * dz * ox2 * oy * oz +
            54 * dx * dz * ox * oy2 * oz + (243 / 2) * dy * dz * oy3 * oz + 18 * dx2 * ox2 * oz2 +
            (27 / 2) * dy2 * ox2 * oz2 + 18 * dz2 * ox2 * oz2 + 54 * dx * dy * ox * oy * oz2 +
            (27 / 2) * dx2 * oy2 * oz2 + (729 / 8) * dy2 * oy2 * oz2 + (81 / 2) * dz2 * oy2 * oz2 +
            24 * dx * dz * ox * oz3 + 54 * dy * dz * oy * oz3 + 3 * dx2 * oz4 + (27 * dy2 * oz4) / 4 +
            15 * dz2 * oz4 - 3 * dz2 * ox2 * oz * r - (27 / 80) * dz2 * oy2 * oz * r -
            6 * dx * dz * ox * oz2 * r - (27 / 40) * dy * dz * oy * oz2 * r - dx2 * oz3 * r -
            (9 / 80) * dy2 * oz3 * r - 18 * dx2 * ox2 * r2 - (27 / 2) * dy2 * ox2 * r2 -
            6 * dz2 * ox2 * r2 - 54 * dx * dy * ox * oy * r2 - (27 / 2) * dx2 * oy2 * r2 -
            (729 / 8) * dy2 * oy2 * r2 - (27 / 2) * dz2 * oy2 * r2 - 24 * dx * dz * ox * oz * r2 -
            54 * dy * dz * oy * oz * r2 - 6 * dx2 * oz2 * r2 - (27 / 2) * dy2 * oz2 * r2 -
            18 * dz2 * oz2 * r2 + 3 * dx2 * r4 + (27 * dy2 * r4) / 4 + 3 * dz2 * r4;
  coef[3] = 20 * dx3 * ox3 + 27 * dx * dy2 * ox3 + 12 * dx * dz2 * ox3 + 81 * dx2 * dy * ox2 * oy +
            (243 / 4) * dy3 * ox2 * oy + 27 * dy * dz2 * ox2 * oy + 27 * dx3 * ox * oy2 +
            (729 / 4) * dx * dy2 * ox * oy2 + 27 * dx * dz2 * ox * oy2 + (243 / 4) * dx2 * dy * oy3 +
            (3645 * dy3 * oy3) / 16 + (243 / 4) * dy * dz2 * oy3 + 36 * dx2 * dz * ox2 * oz +
            27 * dy2 * dz * ox2 * oz + 12 * dz3 * ox2 * oz + 108 * dx * dy * dz * ox * oy * oz +
            27 * dx2 * dz * oy2 * oz + (729 / 4) * dy2 * dz * oy2 * oz + 27 * dz3 * oy2 * oz +
            12 * dx3 * ox * oz2 + 27 * dx * dy2 * ox * oz2 + 36 * dx * dz2 * ox * oz2 +
            27 * dx2 * dy * oy * oz2 + (243 / 4) * dy3 * oy * oz2 + 81 * dy * dz2 * oy * oz2 +
            12 * dx2 * dz * oz3 + 27 * dy2 * dz * oz3 + 20 * dz3 * oz3 - dz3 * ox2 * r -
            (9 / 80) * dz3 * oy2 * r - 6 * dx * dz2 * ox * oz * r - (27 / 40) * dy * dz2 * oy * oz * r -
            3 * dx2 * dz * oz2 * r - (27 / 80) * dy2 * dz * oz2 * r - 12 * dx3 * ox * r2 -
            27 * dx * dy2 * ox * r2 - 12 * dx * dz2 * ox * r2 - 27 * dx2 * dy * oy * r2 -
            (243 / 4) * dy3 * oy * r2 - 27 * dy * dz2 * oy * r2 - 12 * dx2 * dz * oz * r2 -
            27 * dy2 * dz * oz * r2 - 12 * dz3 * oz * r2;
  coef[4] = 15 * dx4 * ox2 + (81 / 2) * dx2 * dy2 * ox2 + (243 * dy4 * ox2) / 16 +
            18 * dx2 * dz2 * ox2 + (27 / 2) * dy2 * dz2 * ox2 + 3 * dz4 * ox2 +
            54 * dx3 * dy * ox * oy + (243 / 2) * dx * dy3 * ox * oy + 54 * dx * dy * dz2 * ox * oy +
            (27 * dx4 * oy2) / 4 + (729 / 8) * dx2 * dy2 * oy2 + (10935 * dy4 * oy2) / 64 +
            (27 / 2) * dx2 * dz2 * oy2 + (729 / 8) * dy2 * dz2 * oy2 + (27 * dz4 * oy2) / 4 +
            24 * dx3 * dz * ox * oz + 54 * dx * dy2 * dz * ox * oz + 24 * dx * dz3 * ox * oz +
            54 * dx2 * dy * dz * oy * oz + (243 / 2) * dy3 * dz * oy * oz + 54 * dy * dz3 * oy * oz +
            3 * dx4 * oz2 + (27 / 2) * dx2 * dy2 * oz2 + (243 * dy4 * oz2) / 16 +
            18 * dx2 * dz2 * oz2 + (81 / 2) * dy2 * dz2 * oz2 + 15 * dz4 * oz2 -
            2 * dx * dz3 * ox * r - (9 / 40) * dy * dz3 * oy * r - 3 * dx2 * dz2 * oz * r -
            (27 / 80) * dy2 * dz2 * oz * r - 3 * dx4 * r2 - (27 / 2) * dx2 * dy2 * r2 -
            (243 * dy4 * r2) / 16 - 6 * dx2 * dz2 * r2 - (27 / 2) * dy2 * dz2 * r2 - 3 * dz4 * r2;
  coef[5] = 6 * dx5 * ox + 27 * dx3 * dy2 * ox + (243 / 8) * dx * dy4 * ox +
            12 * dx3 * dz2 * ox + 27 * dx * dy2 * dz2 * ox + 6 * dx * dz4 * ox + (27 / 2) * dx4 * dy * oy +
            (243 / 4) * dx2 * dy3 * oy + (2187 * dy5 * oy) / 32 + 27 * dx2 * dy * dz2 * oy +
            (243 / 4) * dy3 * dz2 * oy + (27 / 2) * dy * dz4 * oy + 6 * dx4 * dz * oz +
            27 * dx2 * dy2 * dz * oz + (243 / 8) * dy4 * dz * oz + 12 * dx2 * dz3 * oz +
            27 * dy2 * dz3 * oz + 6 * dz5 * oz - dx2 * dz3 * r - (9 / 80) * dy2 * dz3 * r;
  coef[6] = dx6 + (27 * dx4 * dy2) / 4 + (243 * dx2 * dy4) / 16 + (729 * dy6) / 64 +
            3 * dx4 * dz2 + (27 / 2) * dx2 * dy2 * dz2 + (243 * dy4 * dz2) / 16 + 3 * dx2 * dz4 +
            (27 * dy2 * dz4) / 4 + dz6;
  return coef;
}
inline int sign(double x) {
  return x < eps ? -1 : x > eps;
}
inline double get(const std::vector<double> &coef, double t) {
  double e = 1, s = 0;
  for (int i = 0; i < coef.size(); i++) {
    s += e * coef[i];
    e *= t;
  }
  return s;
}
double Find(const std::vector<double> &coef, int n, double l, double r) {
  double sign_l = get(coef, l), sign_r = get(coef, r);
  if (sign_l == 0) return l;
  if (sign_r == 0) return r;
  if (sign_l * sign_r > 0) return -1;
  for (int step = 0; step < 100 && r - l > eps; step++) {
    double m = (l + r) * 0.5;
    int sign_m = sign(get(coef, m));
    if (sign_m == 0) return m;
    if (sign_m * sign_l < 0)
      r = m;
    else
      l = m;
  }
  return (l + r) * 0.5;
}
std::vector<double> Solve(std::vector<double> coef, int n) {
  std::vector<double> ret;
  if (n == 1) {
    if (sign(coef[1])) ret.emplace_back(-coef[0] / coef[1]);
    return ret;
  }
  std::vector<double> dcoef(n);
  for (int i = 0; i < n; i++) dcoef[i] = coef[i + 1] * (i + 1);
  std::vector<double> droot = Solve(dcoef, n - 1);
  droot.insert(droot.begin(), -inf);
  droot.emplace_back(inf);
  for (int i = 0; i < droot.size() - 1; i++) {
    double tmp = Find(coef, n, droot[i], droot[i + 1]);
    if (tmp < inf) ret.emplace_back(tmp);
  }
  return ret;
}
bool Heart::hit(const Ray &r, Float t_min, Float t_max, HitRecord &rec) const {
  Vector3f oc = r.o - center;
  auto coef = PrepareCoef(oc, r.d, scaled, 6);
  auto ans = Solve(coef, 6);
  std::sort(ans.begin(), ans.end());
  for (auto x : ans) {
    if (x > t_min && x < t_max) {
      rec.t = x;
      rec.p = r(rec.t);
      rec.normal = Normalize(rec.p - center);
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }
  return false;
}

bool Heart::BoundingBox(float t0, float t1, AABB &box) const {
  box = AABB(center - Vector3f(scaled, scaled, scaled), center + Vector3f(scaled, scaled, scaled));
  return true;
}

CHIHAYA_NAMESPACE_END