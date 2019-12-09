/*
 * Copyright (C) 2018-2019, neverfelly <neverfelly@gmail.com>
 *
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 */

#include "heart.h"

CHIHAYA_NAMESPACE_BEGIN

#define EPS 1e-6
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
float Function(Vector3f o, Vector3f d, float t) {
	return 1;
}

float Newton(Vector3f o, Vector3f d, float t) {
	double x0, x1;
	x0 = t;
	while (true) {
		x1 = x0 - Function(o, d, x0);
		if (fabs(x1 - x0) < EPS) {
			return x1;
		} else {
			x0 = x1;
		}
	}
}

bool Heart::hit(const Ray &r, Float t_min, Float t_max, HitRecord &rec) const {
  Vector3f oc = r.o - center;
	float tmin = t_min;
	float last_ret = t_min;
	while(true) {
		float ret = Newton(oc, r.d, tmin);
		if (ret < t_min ) {
			return false;
		}
		if (fabs(last_ret - ret) < EPS) 
			break;
	}
	return true;
}

bool Heart::BoundingBox(float t0, float t1, AABB &box) const {
	box = AABB(center - Vector3f(0.1, 0.1, 0.1), center + Vector3f(0.1, 0.1, 0.1))
}
CHIHAYA_NAMESPACE_END