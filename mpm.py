import taichi as ti
ti.init(arch=ti.cuda)
res = 1
n_particles, n_grid = 9000 * res ** 2, 128 * res
dx, inv_dx = 1 / n_grid, float(n_grid)
dt = 1e-4 / res
p_vol, p_rho = (dx * 0.5)**2, 1
p_mass = p_vol * p_rho
E, nu = 0.1e4, 0.2
mu_0, lambda_0 = E / (2 * (1 + nu)), E * nu / ((1+nu) * (1 - 2 * nu))

x = ti.Vector(2, dt=ti.f32, shape=n_particles)
v = ti.Vector(2, dt=ti.f32, shape=n_particles)
C = ti.Matrix(2, 2, dt=ti.f32, shape=n_particles)
F = ti.Matrix(2, 2, dt=ti.f32, shape=n_particles)
material = ti.var(dt=ti.i32, shape=n_particles)
Jp = ti.var(dt=ti.f32, shape=n_particles)
grid_v = ti.Vector(2, dt=ti.f32, shape=(n_grid, n_grid))
grid_m = ti.var(dt=ti.f32, shape=(n_grid, n_grid))

@ti.kernel
def substep():
  for i, j in grid_m:
    grid_v[i, j] = [0, 0]
    grid_m[i, j] = 0
  for p in x:
    base = (x[p] * inv_dx - 0.5).cast(int)
    fx = x[p] * inv_dx - base.cast(float)
    w = [0.5 * ti.sqr(1.5 - fx), 0.75 - ti.sqr(fx - 1), 0.5 * ti.sqr(fx - 0.5)]
    F[p] = (ti.Matrix.identity(ti.f32, 2) + dt * C[p]) @ F[p] 
    h = ti.exp(10 * (1.0 - Jp[p]))
    if material[p] == 0:
      h = 0.3
    mu, la = mu_0 * h, lambda_0 * h
    U, sig, V = ti.svd(F[p])
    J = 1.0
    for d in ti.static(range(2)):
      new_sig = sig[d, d]
      if material[p] == 1: 
        new_sig = min(max(sig[d, d], 1 - 2.5e-2), 1 + 4.5e-3) 
      Jp[p] *= sig[d, d] / new_sig
      sig[d, d] = new_sig
      J *= new_sig
    if material[p] == 1:
      F[p] = U @ sig @ V.T()
    stress = 2 * mu * (F[p] - U @ V.T()) @ F[p].T() + ti.Matrix.identity(ti.f32, 2) * la * J * (J - 1)
    stress = (-dt * p_vol * 4 * inv_dx * inv_dx) * stress
    affine = stress + p_mass * C[p]
    for i, j in ti.static(ti.ndrange(3, 3)):
      offset = ti.Vector([i, j])
      dpos = (offset.cast(float) - fx) * dx
      weight = w[i][0] * w[j][1]
      grid_v[base + offset] += weight * (p_mass * v[p] + affine @ dpos)
      grid_m[base + offset] += weight * p_mass
  for i, j in grid_m:
    if grid_m[i, j] > 0:
      grid_v[i, j] = (1 / grid_m[i, j]) * grid_v[i, j]
      grid_v[i, j][1] -= dt * 50
      if i < 3 and grid_v[i, j][0] < 0:          grid_v[i, j][0] = 0
      if i > n_grid - 3 and grid_v[i, j][0] > 0: grid_v[i, j][0] = 0
      if j < 3 and grid_v[i, j][1] < 0:          grid_v[i, j][1] = 0
      if j > n_grid - 3 and grid_v[i, j][1] > 0: grid_v[i, j][1] = 0
  for p in x: 
    base = (x[p] * inv_dx - 0.5).cast(int)
    fx = x[p] * inv_dx - base.cast(float)
    w = [0.5 * ti.sqr(1.5 - fx), 0.75 - ti.sqr(fx - 1.0), 0.5 * ti.sqr(fx - 0.5)]
    new_v = ti.Vector.zero(ti.f32, 2)
    new_C = ti.Matrix.zero(ti.f32, 2, 2)
    for i, j in ti.static(ti.ndrange(3, 3)):
      dpos = ti.Vector([i, j]).cast(float) - fx
      g_v = grid_v[base + ti.Vector([i, j])]
      weight = w[i][0] * w[j][1]
      new_v += weight * g_v
      new_C += 4 * inv_dx * weight * ti.outer_product(g_v, dpos)
    v[p], C[p] = new_v, new_C
    x[p] += dt * v[p]

import random
import math
for i in range(n_particles):
  xx = random.random() * 3 - 1.5
  yy = random.random() * 3 - 1.5
  aa = xx*xx+yy*yy-1
  while aa*aa*aa-xx*xx*yy*yy*yy>0:
    xx = random.random() * 3 - 1.5
    yy = random.random() * 3 - 1.5
    aa = xx*xx+yy*yy-1
  x[i] = [xx * 0.1 + 0.6 , yy * 0.1 + 0.6]
  material[i] = 1
  v[i] = [5, -2]
  F[i] = [[1, 0], [0, 1]]
  Jp[i] = 1

import numpy as np
gui = ti.GUI("Sim", res=512, background_color=0xFFFFFF)
video = ti.tools.video.VideoManager(output_dir='./')
colors = np.array([0xFF0000, 0xFF8C00, 0x2E8B57], dtype=np.uint32)
files = []
for frame in range(100):
  for s in range(int(2e-3 // dt)):
    substep()
  indices = np.array([int(frame / 100)] * n_particles)
  gui.circles(x.to_numpy(), radius=1.5, color=colors[indices])
  gui.show(f'{frame:06d}.png') # Change to gui.show(f'{frame:06d}.png') to write images to disk
  files.append(f'{frame:06d}.png')
ti.tools.video.make_video(files, width=512, height=512)
ti.tools.video.mp4_to_gif('video.mp4', 'video.gif', 24)
