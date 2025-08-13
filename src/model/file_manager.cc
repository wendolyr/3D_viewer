#include "file_manager.h"

#include <iostream>

namespace s21 {

FileError FileManager::ParseFile(const std::string &file_name,
                                 FigureModel &model) {
  std::ifstream file(file_name);
  if (!file.is_open()) {
    return FileError::kNotExist;
  }

  std::string line;
  std::vector<Vertex> vertices;
  std::unordered_set<std::pair<unsigned, unsigned>, PairHash> edges;
  auto start = std::chrono::steady_clock::now();
  while (std::getline(file, line)) {
    const char *ptr = &line[0];

    if (*ptr == 'v' && *(ptr + 1) == ' ') {
      if (ParseVertices(ptr, vertices) != FileError::kOk) {
        return FileError::kInvalidFile;
      }
    } else if (*ptr == 'f' && *(ptr + 1) == ' ') {
      if (ParseEdges(ptr, vertices, edges) != FileError::kOk) {
        return FileError::kInvalidFile;
      }
    }
  }

  if (vertices.size() == 0) {
    return FileError::kInvalidFile;
  }

  auto end = std::chrono::steady_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Parsing time " << duration.count() << " ms" << std::endl;

  model.SetVertices(vertices);
  model.SetEdges(edges);
  return FileError::kOk;
}

FileError FileManager::ParseVertices(const char *ptr,
                                     std::vector<Vertex> &vertices) {
  ++(++ptr);
  std::vector<double> nums(3, 0.0);
  char *end;
  for (size_t i = 0; i < nums.size(); ++i) {
    nums[i] = std::strtof(ptr, &end);
    if (*end != ' ' && *end != '\0') {
      return FileError::kInvalidFile;
    }

    ptr = end;
  }

  if (*end != '\0') {
    return FileError::kInvalidFile;
  }

  vertices.emplace_back(nums[0], nums[1], nums[2]);
  return FileError::kOk;
}

FileError FileManager::ParseEdges(
    const char *ptr, std::vector<Vertex> &vertices,
    std::unordered_set<std::pair<unsigned, unsigned>, PairHash> &edges) {
  ++(++ptr);
  std::vector<unsigned> face;

  while (*ptr != '\0') {
    if (*ptr == ' ' || *ptr == '\t') {
      ++ptr;
      continue;
    }

    unsigned num = 0;
    bool negative = *ptr == '-';
    if (negative) {
      ++ptr;
    }

    if (*ptr >= '0' && *ptr <= '9') {
      while (*ptr >= '0' && *ptr <= '9') {
        num = num * 10 + (*ptr - '0');
        ++ptr;
      }
    } else {
      return FileError::kInvalidFile;
    }

    while (*ptr == '/' || (*ptr >= '0' && *ptr <= '9')) {
      ++ptr;
    }

    if (*ptr != ' ' && *ptr != '\0') {
      return FileError::kInvalidFile;
    }

    num = negative ? static_cast<unsigned>(vertices.size()) - num : num - 1;

    if (num >= static_cast<unsigned>(vertices.size())) {
      return FileError::kInvalidFile;
    }

    face.push_back(num);
  }

  if (face.size() >= 2) {
    for (size_t i = 0; i < face.size(); ++i) {
      std::pair<unsigned, unsigned> pair;
      size_t prev_ind = i > 0 ? i - 1 : face.size() - 1;
      pair = face[i] > face[prev_ind] ? std::make_pair(face[prev_ind], face[i])
                                      : std::make_pair(face[i], face[prev_ind]);
      edges.insert(pair);
    }
  }

  return FileError::kOk;
}

void FileManager::SaveModel(FigureModel &model, ViewParams &view_params) {
  std::ofstream file(".last_object.obj");
  if (!file.is_open()) {
    return;
  }

  for (const Vertex &v : model.GetVertices()) {
    file << "v " << v.x << ' ' << v.y << ' ' << v.z << std::endl;
  }

  for (const auto &f : model.GetEdges()) {
    file << "f " << f.first + 1 << ' ' << f.second + 1 << std::endl;
  }

  SaveSettings(model, view_params);
}

void FileManager::SaveSettings(FigureModel &model, ViewParams &view_params) {
  std::ofstream file(".last_settings.txt");
  if (!file.is_open()) {
    return;
  }

  Params par = model.GetCurrentSettings();
  file << par.shift.x << ' ' << par.shift.y << ' ' << par.shift.z << std::endl;
  file << par.rotation.x << ' ' << par.rotation.y << ' ' << par.rotation.z
       << std::endl;
  file << par.scale << std::endl;

  file << view_params.edge_color.x << ' ' << view_params.edge_color.y << ' '
       << view_params.edge_color.z << ' ' << std::endl;
  file << view_params.vertex_color.x << ' ' << view_params.vertex_color.y << ' '
       << view_params.vertex_color.z << std::endl;
  file << view_params.background_color.x << ' '
       << view_params.background_color.y << ' '
       << view_params.background_color.z << std::endl;

  file << view_params.projection_type << std::endl;
  file << view_params.edge_type << std::endl;
  file << view_params.edge_thickness << std::endl;
  file << view_params.vertex_display << std::endl;
  file << view_params.vertex_size << std::endl;
  file << view_params.file_name << std::endl;
}

bool FileManager::LoadLastState(FigureModel &model, ViewParams &view_params) {
  bool is_loaded = true;
  FigureModel temp_m;
  ViewParams temp_v;
  if (ParseFile(".last_object.obj", temp_m) == FileError::kOk) {
    if (LoadSettings(temp_m, temp_v)) {
      model = std::move(temp_m);
      view_params = std::move(temp_v);
    } else {
      is_loaded = false;
    }
  } else {
    if (LoadSettings(temp_m, temp_v)) {
      view_params = std::move(temp_v);
    } else {
      is_loaded = false;
    }
  }

  return is_loaded;
}

bool FileManager::LoadSettings(FigureModel &model, ViewParams &view_params) {
  std::ifstream file(".last_settings.txt");
  if (!file.is_open()) {
    return false;
  }

  bool is_loaded = true;

  Params temp_p;
  ViewParams temp_vp;

  is_loaded = LoadTripleSetting(temp_p.shift, file) &&
              LoadTripleSetting(temp_p.rotation, file) &&
              LoadSingleSetting(temp_p.scale, file) &&
              LoadTripleSetting(temp_vp.edge_color, file) &&
              LoadTripleSetting(temp_vp.vertex_color, file) &&
              LoadTripleSetting(temp_vp.background_color, file) &&
              LoadSingleSetting(temp_vp.projection_type, file) &&
              LoadSingleSetting(temp_vp.edge_type, file) &&
              LoadSingleSetting(temp_vp.edge_thickness, file) &&
              LoadSingleSetting(temp_vp.vertex_display, file) &&
              LoadSingleSetting(temp_vp.vertex_size, file) &&
              LoadSingleSetting(temp_vp.file_name, file);

  if (is_loaded) {
    model.SetSettings(temp_p);
    view_params = std::move(temp_vp);
  }

  return is_loaded;
}

bool FileManager::LoadTripleSetting(Vertex &p, std::ifstream &file) {
  std::string extra;
  std::string line;

  if (!std::getline(file, line)) {
    return false;
  }

  std::istringstream input(line);
  if ((input >> p.x >> p.y >> p.z) && !(input >> extra)) {
    return true;
  }

  return false;
}

template <typename T>
bool FileManager::LoadSingleSetting(T &p, std::ifstream &file) {
  std::string extra;
  std::string line;

  if (!std::getline(file, line)) {
    return false;
  }

  std::istringstream input(line);
  if ((input >> p) && !(input >> extra)) {
    return true;
  }

  return false;
}

}  // namespace s21