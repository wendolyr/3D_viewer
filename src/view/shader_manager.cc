#include "shader_manager.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

namespace s21 {
// public
ShaderManager::ShaderManager()
    : line_shader_(nullptr), point_shader_(nullptr) {}

ShaderManager::~ShaderManager() {
  delete line_shader_;
  delete point_shader_;
}

bool ShaderManager::Initialize() {
  return CreateLineShader() && CreatePointShader();
}

QOpenGLShaderProgram* ShaderManager::GetShader(ShaderType type) {
  switch (type) {
    case kLINE:
      return line_shader_;
    case kPOINT:
      return point_shader_;
    default:
      return nullptr;
  }
}

QString ShaderManager::LoadShaderSource(const QString& file_path) {
  QFile file(file_path);
  QFileInfo fileInfo(file);
  qDebug() << "Looking for shader at:" << fileInfo.absoluteFilePath();
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Could not open shader file:" << file_path;
    return QString();
  }
  QTextStream stream(&file);
  return stream.readAll();
}
// private
bool ShaderManager::CreateLineShader() {
  line_shader_ = new QOpenGLShaderProgram();

  QString vert_source = LoadShaderSource("view/shaders/line.vert");
  QString geom_source = LoadShaderSource("view/shaders/line.geom");
  QString frag_source = LoadShaderSource("view/shaders/line.frag");

  if (vert_source.isEmpty() || geom_source.isEmpty() || frag_source.isEmpty()) {
    qWarning() << "Failed to load line shader sources";
    return false;
  }

  if (!line_shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                             vert_source) ||
      !line_shader_->addShaderFromSourceCode(QOpenGLShader::Geometry,
                                             geom_source) ||
      !line_shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                             frag_source) ||
      !line_shader_->link()) {
    qWarning() << "Line shader compilation/linking failed:"
               << line_shader_->log();
    return false;
  }

  return true;
}

bool ShaderManager::CreatePointShader() {
  point_shader_ = new QOpenGLShaderProgram();

  // Загрузка шейдеров из файлов
  QString vert_source = LoadShaderSource("view/shaders/point.vert");
  QString frag_source = LoadShaderSource("view/shaders/point.frag");

  if (vert_source.isEmpty() || frag_source.isEmpty()) {
    qWarning() << "Failed to load line shader sources";
    return false;
  }

  if (!point_shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                              vert_source) ||
      !point_shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                              frag_source) ||
      !point_shader_->link()) {
    qWarning() << "Line shader compilation/linking failed:"
               << point_shader_->log();
    return false;
  }

  return true;
}
}  // namespace s21