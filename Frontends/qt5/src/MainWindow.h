#pragma once

#include <chrono>
#include <memory>

#include <QMainWindow>
#include <QTimer>

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

  std::chrono::time_point<std::chrono::steady_clock> fpsTimer;

  bool         running          = false;
  int          framerate        = 60;
  int          elapsedFrames    = 0;
  double       volume           = 0.1;
  std::uint8_t controller_state = 0;
};
