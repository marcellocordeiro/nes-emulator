#include "QtOpenGL.h"

QtOpenGL::QtOpenGL(QWidget* parent) : QOpenGLWidget(parent) {}

void QtOpenGL::setSize(int w, int h)
{
  width  = w;
  height = h;
}

void QtOpenGL::setBuffer(const uint32_t* ptr) { buffer = ptr; }

void QtOpenGL::initializeGL()
{
  initializeOpenGLFunctions();

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DITHER);
  glDisable(GL_POLYGON_SMOOTH);
  glDisable(GL_STENCIL_TEST);

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, width);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
}

void QtOpenGL::resizeGL(int w, int h)
{
  auto ratio = devicePixelRatio();
  glViewport(0, 0, w * ratio, h * ratio);
}

void QtOpenGL::paintGL()
{
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);

  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1.0f, -1.0f);

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1.0f, -1.0f);

  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1.0f, 1.0f);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1.0f, 1.0f);

  glEnd();
}
