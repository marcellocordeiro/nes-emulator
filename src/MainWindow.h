#pragma once

#include <memory>

#include <QMainWindow>
#include <QTimer>

#include "Sound_Queue.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private Q_SLOTS:
  void run_frame();

private:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

  void resize(int);

  std::unique_ptr<Ui::MainWindow> ui;
  std::unique_ptr<QTimer>         timer;
  int                             framerate  = 60;

  std::uint8_t controller_state = 0;

  // TODO: reimplement this
  std::array<std::int16_t, 4096> audio_buffer = {};
  std::unique_ptr<Sound_Queue>   sound_queue;
};
