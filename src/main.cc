#include <QApplication>
#include <QSurfaceFormat>

#include "view/main_widget.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  // Настройка OpenGL
  QSurfaceFormat format;
  format.setVersion(4, 1);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  format.setSamples(4);
  format.setSwapInterval(1);
  QSurfaceFormat::setDefaultFormat(format);

  // Создание и отображение главного окна
  s21::MainWidget window;
  window.setWindowTitle("3D_Viewer");
  window.resize(1350, 1000);
  window.show();

  return app.exec();
}