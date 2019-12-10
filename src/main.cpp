#include <iostream>
#include <string>
#include <vector>

#include "core/box.h"
#include "core/camera.h"
#include "core/common.h"
#include "core/heart.h"
#include "core/hitable_list.h"
#include "core/image.h"
#include "core/material.h"
#include "core/ray.h"
#include "core/rect.h"
#include "core/sphere.h"
#include "core/vector3.h"

using namespace chihaya;

inline Vector3f Denan(const Vector3f &c) {
  Vector3f temp = c;
  if (!(temp[0] == temp[0])) temp[0] = 0;
  if (!(temp[1] == temp[1])) temp[1] = 0;
  if (!(temp[2] == temp[2])) temp[2] = 0;
  return temp;
}

Vector3f color(const Ray &r, Hitable *world, int depth) {
  HitRecord rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    Ray scattered;
    Vector3f attenuation;
    Vector3f emitted = rec.mat_ptr->Emitted(rec.u, rec.v, rec.p);
    if (depth < 50 && rec.mat_ptr->Scatter(r, rec, attenuation, scattered)) {
      return emitted + attenuation * color(scattered, world, depth + 1);
    } else {
      return emitted;
    }
  } else {
    return Vector3f(0, 0, 0);
    //Vector3f unit = Normalize(r.d);
    //Float t = 0.5 * (unit.y + 1.0);
    //return Vector3f(1, 1, 1) * (1.0 - t) + Vector3f(0.5, 0.7, 1.0) * t;
  }
}

//Hitable *RandomScene() {
//  int n = 500;
//  Hitable **list = new Hitable *[n + 1];
//  list[0] = new Sphere(Vector3f(0, -1000, 0), 1000, new Lambertian(Vector3f(0.5, 0.5, 0.5)));
//  int i = 1;
//  for (int a = -11; a < 11; a++) {
//    for (int b = -11; b < 11; b++) {
//      float choose_mat = drand48();
//      Vector3f center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
//      if ((center - Vector3f(4, 0.2, 0)).Length() > 0.9) {
//        if (choose_mat < 0.8) {  // diffuse
//          list[i++] = new Sphere(
//              center, 0.2,
//              new Lambertian(Vector3f(drand48() * drand48(),
//                                      drand48() * drand48(),
//                                      drand48() * drand48())));
//        } else if (choose_mat < 0.95) {  // metal
//          list[i++] = new Sphere(
//              center, 0.2,
//              new Metal(Vector3f(0.5 * (1 + drand48()),
//                                 0.5 * (1 + drand48()),
//                                 0.5 * (1 + drand48())),
//                        0.5 * drand48()));
//        } else {  // glass
//          list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
//        }
//      }
//    }
//  }

//list[i++] = new Sphere(Vector3f(0, 1, 0), 1.0, new Dielectric(1.5));
//list[i++] = new Sphere(Vector3f(-4, 1, 0), 1.0, new Lambertian(Vector3f(0.4, 0.2, 0.1)));
//list[i++] = new Sphere(Vector3f(4, 1, 0), 1.0, new Metal(Vector3f(0.7, 0.6, 0.5), 0.0));
//
//return new HitableList(list, i);
//}

Hitable *CornellBox() {
  Hitable **list = new Hitable *[8];
  int i = 0;
  Material *red = new Lambertian(new ConstantTexture(Vector3f(0.65, 0.05, 0.05)));
  Material *white = new Lambertian(new ConstantTexture(Vector3f(0.73, 0.73, 0.73)));
  Material *green = new Lambertian(new ConstantTexture(Vector3f(0.12, 0.45, 0.15)));
  Material *light = new DiffuseLight(new ConstantTexture(Vector3f(15, 15, 15)));
  Material *glass = new Dielectric(1.5);
  list[i++] = new FlipNormals(new YZrect(0, 555, 0, 555, 555, green));
  list[i++] = new YZrect(0, 555, 0, 555, 0, red);
  list[i++] = new XZrect(213, 343, 227, 332, 554, light);

  list[i++] = new FlipNormals(new XZrect(0, 555, 0, 555, 555, white));
  list[i++] = new XZrect(0, 555, 0, 555, 0, white);
  list[i++] = new FlipNormals(new XYrect(0, 555, 0, 555, 555, white));
  //list[i++] = new Translate(new RotateY(new Box(Vector3f(0, 0, 0), Vector3f(165, 165, 165), white), -18), Vector3f(130, 0, 65));
  list[i++] = new Heart(Vector3f(190, 90, 190), 90, red);
  list[i++] = new Translate(new RotateY(new Box(Vector3f(0, 0, 0), Vector3f(165, 330, 165), white), 15), Vector3f(265, 0, 295));
  return new HitableList(list, i);
}
int main(int argc, char *argv[]) {
  int nx = 400;
  int ny = 400;
  int ns = 800;
  std::cout << "Start" << std::endl;
  chihaya::Image image(nx, ny, 3);
  //Hitable *world = RandomScene();
  Hitable *world = CornellBox();
  Vector3f lookfrom(278, 278, -800);
  Vector3f lookat(278, 278, 0);
  Float dist_to_focus = 10.0;
  Float aperture = 0.0;
  Camera camera(lookfrom, lookat, Vector3f(0, 1, 0), 40, Float(nx) / Float(ny), aperture, dist_to_focus);
  for (int j = ny - 1; j >= 0; --j) {
    for (int i = 0; i < nx; ++i) {
      Vector3f col(0, 0, 0);
      for (int s = 0; s < ns; ++s) {
        Float u = Float(i + drand48()) / Float(nx);
        Float v = Float(j + drand48()) / Float(ny);
        Ray r = camera.GetRay(u, v);
        Vector3f p = r(2);
        col += Denan(color(r, world, 0));
      }
      std::cout << i << " " << j << std::endl;
      col /= ns;
      col = Vector3f(sqrt(col.x), sqrt(col.y), sqrt(col.z));
      auto ir = static_cast<unsigned char>(255 * std::clamp(col.x, 0.0f, 1.0f));
      auto ig = static_cast<unsigned char>(255 * std::clamp(col.y, 0.0f, 1.0f));
      auto ib = static_cast<unsigned char>(255 * std::clamp(col.z, 0.0f, 1.0f));
      image.SetPixel(i, j, ir, ig, ib);
    }
  }
  image.Save("out.png", true);
  std::cout << "Finish" << std::endl;
  return 0;
}