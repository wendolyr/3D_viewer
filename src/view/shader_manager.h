#ifndef CPP4_3DVIEWER_V2_0_VIEW_SHADER_MANAGER_H
#define CPP4_3DVIEWER_V2_0_VIEW_SHADER_MANAGER_H

#include <QOpenGLShaderProgram>
#include <QString>

namespace s21 {
/**
 * @class ShaderManager
 * @brief Менеджер для управления шейдерными программами OpenGL
 * @details Отвечает за загрузку, компиляцию и управление шейдерными программами
 */
class ShaderManager {
 public:
  /**
   * @enum ShaderType
   * @brief Типы шейдерных программ
   */
  enum ShaderType { kLINE, kPOINT };
  /**
   * @brief Конструктор по умолчанию
   */
  ShaderManager();
  /**
   * @brief Деструктор, унитожает шейдерные программы
   */
  ~ShaderManager();
  /**
   * @brief Инициализирует шейдерные программы
   * @return true если инициализация прошла успешно, иначе false
   */
  bool Initialize();
  /**
   * @brief Получает шейдерную программу по типу
   * @param type Тип шейдерной программы
   * @return Указатель на запрошенную шейдерную программу или nullptr
   */
  QOpenGLShaderProgram* GetShader(ShaderType type);
  /**
   * @brief Загружает исходный код шейдера из файла
   * @param file_path Путь к файлу шейдера
   * @return Содержимое файла шейдера или пустая строка при ошибке
   */
  static QString LoadShaderSource(const QString& file_path);

 private:
  QOpenGLShaderProgram*
      line_shader_;  ///< Шейдерная программа для отрисовки линий
  QOpenGLShaderProgram*
      point_shader_;  ///< Шейдерная программа для отрисовки точек
  /**
   * @brief Создает шейдерную программу для линий
   * @return true если создание прошло успешно, иначе false
   */
  bool CreateLineShader();
  /**
   * @brief Создает шейдерную программу для точек
   * @return true если создание прошло успешно, иначе false
   */
  bool CreatePointShader();
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_SHADER_MANAGER_H