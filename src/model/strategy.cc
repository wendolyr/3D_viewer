#include "strategy.h"

#include <iostream>

namespace s21 {

void ScaleStrategy::Transform(std::vector<Vertex> &vertices,
                              const Vertex scale) {
  // for (Vertex i : vertices) {
  //   i.x *= scale.x;
  //   i.y *= scale.x;
  //   i.z *= scale.x;
  // }

  auto worker = [&](size_t start, size_t end) {
    for (size_t j = start; j < end; ++j) {
      Vertex &i = vertices[j];
      i.x *= scale.x;
      i.y *= scale.x;
      i.z *= scale.x;
    }
  };

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

void MoveStrategy::Transform(std::vector<Vertex> &vertices, const Vertex axis) {
  // for (Vertex i : vertices) {
  //   i.x += axis.x;
  //   i.y += axis.y;
  //   i.z += axis.z;
  // }

  auto worker = [&](size_t start, size_t end) {
    for (size_t j = start; j < end; ++j) {
      Vertex &i = vertices[j];
      i.x += axis.x;
      i.y += axis.y;
      i.z += axis.z;
    }
  };

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

void RotateStrategy::Transform(std::vector<Vertex> &vertices,
                               const Vertex axis) {
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
      if (rad_angle_x > 1e-7) {
        i.y = i.y * calc_cos_x - i.z * calc_sin_x;
        i.z = i.y * calc_sin_x + i.z * calc_cos_x;
      } else if (rad_angle_y > 1e-7) {
        i.x = i.x * calc_cos_y + i.z * calc_sin_y;
        i.z = -i.x * calc_sin_y + i.z * calc_cos_y;
      } else {
        i.x = i.x * calc_cos_z - i.y * calc_sin_z;
        i.y = i.x * calc_sin_z + i.y * calc_cos_z;
      }
    }
  };

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

}  // namespace s21