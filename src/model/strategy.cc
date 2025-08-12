#include "strategy.h"

#include <iostream>

namespace s21 {

void TransformStrategy::ParallelTransform(
    std::vector<Vertex> &vertices,
    const std::function<void(size_t, size_t)> &worker) {
  const size_t num_threads = std::thread::hardware_concurrency();
  const size_t chunk_size = vertices.size() / num_threads;
  std::vector<std::thread> threads;

  for (size_t i = 0; i < num_threads; ++i) {
    size_t start = i * chunk_size;
    size_t end = (i == num_threads - 1) ? vertices.size() : start + chunk_size;
    threads.emplace_back(worker, start, end);
  }

  for (auto &thread : threads) {
    thread.join();
  }
}

void ScaleStrategy::Transform(std::vector<Vertex> &vertices,
                              const Vertex scale) {
  std::cout << "Scale\n";

  auto worker = [&](size_t start, size_t end) {
    for (size_t j = start; j < end; ++j) {
      Vertex &i = vertices[j];
      i.x *= scale.x;
      i.y *= scale.x;
      i.z *= scale.x;
    }
  };

  ParallelTransform(vertices, worker);
}

void MoveStrategy::Transform(std::vector<Vertex> &vertices, const Vertex axis) {
  std::cout << "Move\n";

  auto worker = [&](size_t start, size_t end) {
    for (size_t j = start; j < end; ++j) {
      Vertex &i = vertices[j];
      i.x += axis.x;
      i.y += axis.y;
      i.z += axis.z;
    }
  };

  ParallelTransform(vertices, worker);
}

void RotateStrategy::Transform(std::vector<Vertex> &vertices,
                               const Vertex axis) {
  std::cout << "Rotate\n";
  float rad_angle_x = axis.x * M_PI / 180;
  float rad_angle_y = axis.y * M_PI / 180;
  float rad_angle_z = axis.z * M_PI / 180;

  float calc_sin_x = sin(rad_angle_x);
  float calc_cos_x = cos(rad_angle_x);

  float calc_sin_y = sin(rad_angle_y);
  float calc_cos_y = cos(rad_angle_y);

  float calc_sin_z = sin(rad_angle_z);
  float calc_cos_z = cos(rad_angle_z);

  auto worker = [&](size_t start, size_t end) {
    for (size_t j = start; j < end; ++j) {
      Vertex &i = vertices[j];
      if (fabs(rad_angle_x) > 1e-7) {
        float old_y = i.y;
        float old_z = i.z;
        i.y = old_y * calc_cos_x - old_z * calc_sin_x;
        i.z = old_y * calc_sin_x + old_z * calc_cos_x;
      } else if (fabs(rad_angle_y) > 1e-7) {
        float old_x = i.x;
        float old_z = i.z;
        i.x = old_x * calc_cos_y + old_z * calc_sin_y;
        i.z = -old_x * calc_sin_y + old_z * calc_cos_y;
      } else {
        float old_x = i.x;
        float old_y = i.y;
        i.x = old_x * calc_cos_z - old_y * calc_sin_z;
        i.y = old_x * calc_sin_z + old_y * calc_cos_z;
      }
    }
  };

  ParallelTransform(vertices, worker);
}

}  // namespace s21