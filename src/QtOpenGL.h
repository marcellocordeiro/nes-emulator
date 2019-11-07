#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class QtOpenGL final : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  explicit QtOpenGL(QWidget* parent = nullptr);

private:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  static constexpr int nes_width = 256;
  static constexpr int nes_height = 240;

  GLuint texture = 0;
};
