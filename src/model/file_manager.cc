#include "file_manager.h"

namespace s21 {

int FileManager::ParseFile(std::string &file_name, FigureModel &model) {
  /**
   * обычное чтение через fstream
   * делегировать методам ParseVertices и ParsePolygons для декомпозиции
   * коды ошибок возвращать для вывода ошибок во фронте
   */
}

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
  if (file.is_open()) {
    for (const Vertex &v : model.GetVertices()) {
      file << "v  " << v.x << ' ' << v.y << ' ' << v.z << std::endl;
    }

    for (const auto &f : model.GetPolygons()) {
      file << "f " << f.first << ' ' << f.second << std::endl;
    }

    file.close();
  }

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
}

}  // namespace s21