#include "file_manager.h"

#include <iostream>

namespace s21 {

int FileManager::ParseFile(std::string &file_name, FigureModel &model) {
  std::ifstream file(file_name);
  if (!file.is_open()) {
    return 1;
  }

  std::string line;
  std::vector<Vertex> vertices;
  std::set<std::pair<unsigned, unsigned>> polygons;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "v") {
      Vertex v;
      if (!(iss >> v.x >> v.y >> v.z)) {
        return 2;
      }
      vertices.push_back(v);

    } else if (prefix == "f") {
      std::vector<unsigned> face;
      std::string st;
      while (iss >> st) {
        int num;
        auto [ptr, ec] = std::from_chars(st.data(), st.data() + st.size(), num);

        if (ec != std::errc()) {
          return 2;
        }

        bool negative = !st.empty() && st[0] == '-';
        num = negative ? static_cast<int>(vertices.size()) - num : num - 1;

        if (num < 0 || num >= static_cast<int>(vertices.size())) {
          return 2;
        }
        face.push_back(num);
      }

      if (face.size() >= 2) {
        for (size_t i = 0; i < face.size(); ++i) {
          std::pair<unsigned, unsigned> pair;
          size_t prev_ind = i > 0 ? i - 1 : face.size() - 1;
          pair = face[i] > face[prev_ind]
                     ? std::make_pair(face[prev_ind], face[i])
                     : std::make_pair(face[i], face[prev_ind]);
          polygons.insert(pair);
        }
      }
    }
  }

  model.SetVertices(vertices);
  model.SetPolygons(polygons);

  return 0;
}

// int MainWidget::LoadModelData(const QString &file_path) {
//   while (ptr < end) {
//     // Пропуск пробелов и управляющих символов
//     while (ptr < end &&
//            (*ptr == ' ' || *ptr == '\t' || *ptr == '\r' || *ptr == '\n')) {
//       ++ptr;
//     }
//     if (ptr >= end) break;
//     // Обработка вершин (строки начинающиеся с "v ")
//     if (*ptr == 'v' && (ptr + 1 < end) &&
//         (*(ptr + 1) == ' ' || *(ptr + 1) == '\t')) {
//       if (ptr + 2 >= end) break;
//       const char *line_start = ptr + 2;
//       float x, y, z;
//       char *next;  // Указатель на следующую позицию
//       // Парсинг координаты X
//       const char *before_x = line_start;
//       x = std::strtof(line_start, &next);
//       y = std::strtof(next, &next);
//       z = std::strtof(next, &next);

//       if (next == line_start) {
//         QMessageBox::warning(this, "Ошибка", "Некорректный файл");
//         return 1;
//       }
//       // Добавление вершины
//       vertices.append(QVector3D(x, y, z));
//       ptr = SkipToNextLine(ptr, end);
//       continue;
//     }
//     // New Обработка граней (f)
//     else if (*ptr == 'f' && (ptr + 1 < end) &&
//              (*(ptr + 1) == ' ' || *(ptr + 1) == '\t')) {
//       if (ptr + 2 >= end) break;

//       const char *line_start = ptr + 2;
//       face.clear();

//       // Парсинг каждого значения в строке грани
//       const char *token_start = line_start;
//       // while (token_start < end && *token_start != '\n' && *token_start !=
//       // '\r') {
//       while (token_start < end) {
//         // Пропуск пробелов в начале токена
//         while (token_start < end &&
//                (*token_start == ' ' || *token_start == '\t')) {
//           ++token_start;
//         }
//         if (token_start >= end || *token_start == '\n' || *token_start ==
//         '\r')
//           break;
//         // new variant
//         int idx = 0;
//         bool negative = false;
//         const char *num_start = token_start;

//         // Обработка знака
//         if (*num_start == '-') {
//           negative = true;
//           ++num_start;
//         } else if (*num_start == '+') {
//           ++num_start;
//         }

//         // Парсинг числа
//         if (num_start < end && *num_start >= '0' && *num_start <= '9') {
//           while (num_start < end && *num_start >= '0' && *num_start <= '9') {
//             idx = idx * 10 + (*num_start++ - '0');
//           }

//           // Преобразование индекса
//           unsigned vertex_index;
//           if (negative) {
//             vertex_index = static_cast<unsigned>(vertices.size() - idx);
//           } else {
//             vertex_index = static_cast<unsigned>(idx - 1);
//           }

//           // Проверка и добавление индекса
//           if (vertex_index < static_cast<unsigned>(vertices.size())) {
//             face.push_back(vertex_index);
//           }
//         }

//         while (token_start < end && *token_start != ' ' &&
//                *token_start != '\t' && *token_start != '\n' &&
//                *token_start != '\r') {
//           ++token_start;
//         }
//       }

//       // Сохраняем грань если есть хотя бы 3 вершины
//       const size_t face_size = face.size();
//       // if (face.size() >= 2) {
//       if (face_size >= 2) {
//         // Для каждой вершины в грани
//         // new variant
//         const unsigned int last_index = face[face_size - 1];
//         unsigned int prev_index = last_index;

//         for (size_t i = 0; i < face_size; ++i) {
//           const unsigned int current_index = face[i];

//           // Создание упорядоченной пары
//           const auto edge = (prev_index < current_index)
//                                 ? qMakePair(prev_index, current_index)
//                                 : qMakePair(current_index, prev_index);

//           edges.insert(edge);
//           prev_index = current_index;
//         }
//       }

//       ptr = SkipToNextLine(ptr, end);
//       continue;
//     }
//     // Пропуск остальных строк
//     else {
//       ptr = SkipToNextLine(ptr, end);
//     }
//     // Переход к следующему символу
//     if (ptr < end) ++ptr;
//   }

//   QVector<QPair<unsigned, unsigned>> edgeVec;
//   edgeVec.reserve(edges.size());
//   for (const auto &edge : edges) {
//     edgeVec.append(edge);
//   }

//   return 0;
// }
// //============

void FileManager::LoadLastState(FigureModel &model) {
  std::string last_object_file = ".last_object.obj";
  if (!ParseFile(last_object_file, model)) {
    LoadSettings(model);
  }
}

const char *SkipToTheNextLine(const char *ptr, const char *end) {
  while (ptr < end && *ptr != '\n') {
    ++ptr;
  }

  return ptr;
}

void FileManager::SaveModel(std::string &file_name, FigureModel &model) {
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
 * настройки также должны будут сохранять и загружать толщину, цвет и тд линий и
 * вершин! позже доделать этот момент
 */
void FileManager::SaveSettings(std::string &file_name, FigureModel &model) {
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