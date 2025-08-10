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
  std::unordered_set<std::pair<unsigned, unsigned>, PairHash> polygons;
  auto start = std::chrono::steady_clock::now();
  while (std::getline(file, line)) {
    const char *ptr = &line[0];

    if (*ptr == 'v' && *(ptr + 1) == ' ') {
      if (ParseVertices(ptr, vertices) != FileError::kOk) {
        return FileError::kInvalidFile;
      }
    } else if (*ptr == 'f' && *(ptr + 1) == ' ') {
      if (ParsePolygons(ptr, vertices, polygons) != FileError::kOk) {
        return FileError::kInvalidFile;
      }
    }
  }
  auto end = std::chrono::steady_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Parsing time " << duration.count() << " ms" << std::endl;

  model.SetVertices(vertices);
  model.SetPolygons(polygons);

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

FileError FileManager::ParsePolygons(
    const char *ptr, std::vector<Vertex> &vertices,
    std::unordered_set<std::pair<unsigned, unsigned>, PairHash> &polygons) {
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
      polygons.insert(pair);
    }
  }

  return FileError::kOk;
}

void FileManager::LoadLastState(FigureModel &model) {
  std::string last_object_file = ".last_object.obj";
  if (ParseFile(last_object_file, model) == FileError::kOk) {
    LoadSettings(model);
  }
}

void FileManager::SaveModel(const std::string &file_name, FigureModel &model) {
  std::ofstream file(file_name);
  if (!file.is_open()) {
    return;
  }

  for (const Vertex &v : model.GetVertices()) {
    file << "v  " << v.x << ' ' << v.y << ' ' << v.z << std::endl;
  }

  for (const auto &f : model.GetPolygons()) {
    file << "f " << f.first << ' ' << f.second << std::endl;
  }

  file.close();

  std::string settings_file_name = ".last_settings.txt";
  SaveSettings(settings_file_name, model);
}

/**
 * настройки также должны будут сохранять и загружать толщину, цвет и тд линий
 * и вершин! позже доделать этот момент
 */
void FileManager::SaveSettings(const std::string &file_name,
                               FigureModel &model) {
  std::ofstream file(file_name);
  if (file.is_open()) {
    Params par = model.GetCurrentSettings();
    file << par.shift.x << ' ' << par.shift.y << ' ' << par.shift.z
         << std::endl;
    file << par.rotation.x << ' ' << par.rotation.y << ' ' << par.rotation.z
         << std::endl;
    file << par.scale << std::endl;
    file.close();
  }
}

bool FileManager::LoadSettings(FigureModel &model) {
  std::ifstream file(".last_settings.txt");
  if (!file.is_open()) {
    return false;
  }

  Params temp;
  std::string extra;
  std::string line;

  // сдвиг
  if (!std::getline(file, line)) {
    return false;
  }
  std::istringstream input1(line);
  if (!(input1 >> temp.shift.x >> temp.shift.y >> temp.shift.z)) {
    return false;
  }
  if (input1 >> extra) {
    return false;
  }

  // вращение
  if (!std::getline(file, line)) {
    return false;
  }
  std::istringstream input2(line);
  if (!(input2 >> temp.rotation.x >> temp.rotation.y >> temp.rotation.z)) {
    return false;
  }
  if (input2 >> extra) {
    return false;
  }

  // масштаб
  if (!std::getline(file, line)) {
    return false;
  }
  std::istringstream input3(line);
  if (!(input3 >> temp.scale)) {
    return false;
  }
  if (input2 >> extra) {
    return false;
  }

  if (std::getline(file, line)) {
    return false;
  }

  model.SetSettings(temp);

  return true;
}

}  // namespace s21