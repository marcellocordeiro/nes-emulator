#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class QtOpenGL final : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  explicit QtOpenGL(QWidget* parent = nullptr);

  void setSize(int w, int h);
  void setBuffer(const uint32_t* ptr);

private:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  int width  = 0;
  int height = 0;

  GLuint texture = 0;

  const uint32_t* buffer = nullptr;
};
