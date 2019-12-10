/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#include "heart.h"

CHIHAYA_NAMESPACE_BEGIN

const double eps = 1e-12;
const double inf = 1e12;
/* 
-((-(ox + dx t)^2 (oz + dz t)^3 - 
     9/80 (oy + dy t)^2 (oz + dz t)^3 + (-1 + (ox + dx t)^2 + 
       9/4 (oy + dy t)^2 + (oz + dz t)^2)^3)/(3 dz (ox + dx t)^2 (oz +
         dz t)^2 + 27/80 dz (oy + dy t)^2 (oz + dz t)^2 + 
     2 dx (ox + dx t) (oz + dz t)^3 + 
     9/40 dy (oy + dy t) (oz + dz t)^3 - 
     3 (2 dx (ox + dx t) + 9/2 dy (oy + dy t) + 
        2 dz (oz + dz t)) (-1 + (ox + dx t)^2 + 
        9/4 (oy + dy t)^2 + (oz + dz t)^2)^2))
*/
//float Function(Vector3f o, Vector3f d, float t) {
//  return 1;
//}
//
//float Newton(Vector3f o, Vector3f d, float t) {
//  double x0, x1;
//  x0 = t;
//  while (true) {
//    x1 = x0 - Function(o, d, x0);
//    if (fabs(x1 - x0) < EPS) {
//      return x1;
//    } else {
//      x0 = x1;
//    }
//  }
//}

std::vector<double> PrepareCoef(Vector3f o, Vector3f d, float r, int n) {
  std::vector<double> coef(n + 1);
  coef[0] = Dot(o, o) / (r * r) - 1;
  coef[1] = 2 * Dot(o, d) / (r * r);
  coef[2] = Dot(d, d) / (r * r);
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
  auto coef = PrepareCoef(oc, r.d, scaled, 2);
  auto ans = Solve(coef, 2);
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
  //float tmin = t_min;
  //float last_ret = t_min;
  //while (true) {
  //  float ret = Newton(oc, r.d, tmin);
  //  if (ret < t_min) {
  //    return false;
  //  }
  //  if (fabs(last_ret - ret) < EPS)
  //    break;
  //}
  //return true;
}

bool Heart::BoundingBox(float t0, float t1, AABB &box) const {
  box = AABB(center - Vector3f(scaled, scaled, scaled), center + Vector3f(scaled, scaled, scaled));
  return true;
}

CHIHAYA_NAMESPACE_END