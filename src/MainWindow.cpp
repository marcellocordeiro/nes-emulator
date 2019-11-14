#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <algorithm>

#include <fmt/format.h>
#include <QCoreApplication>
#include <QKeyEvent>

#include <nes/Emulator.h>

using namespace std::chrono_literals;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(std::make_unique<Ui::MainWindow>()),
      timer(std::make_unique<QTimer>(this))
{
  if (QCoreApplication::arguments().size() == 1) {
    throw std::invalid_argument("Too few arguments");
  }

  QSurfaceFormat format;
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setSwapInterval(1);
  format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
  QSurfaceFormat::setDefaultFormat(format);

  ui->setupUi(this);
  resize(3);

  setWindowTitle(Emulator::title);
  ui->video->setSize(Emulator::width, Emulator::height);
  ui->video->setBuffer(Emulator::get_back_buffer());

  Emulator::set_app_path(QCoreApplication::applicationDirPath().toStdString());
  Emulator::load(QCoreApplication::arguments().at(1).toStdString());
  Emulator::power_on();

  connect(timer.get(), &QTimer::timeout, this, &MainWindow::run_frame);
  timer->start(1000 / framerate);

  running  = true;
  fpsTimer = std::chrono::steady_clock::now();
}

MainWindow::~MainWindow() { Emulator::power_off(); }

void MainWindow::run_frame()
{
  if (!running) return;

  Emulator::update_controller_state(0, controller_state);
  Emulator::run_frame();
  ++elapsedFrames;

  ui->video->update();

  auto elapsedTime = std::chrono::steady_clock::now() - fpsTimer;

  if (elapsedTime > 1s) {
    auto fps =
        elapsedFrames / std::chrono::duration<double>(elapsedTime).count();
    auto title = fmt::format("{} | {:5.2f}fps", Emulator::title, fps);
    setWindowTitle(title.c_str());

    fpsTimer      = std::chrono::steady_clock::now();
    elapsedFrames = 0;
  }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
  if (event->isAutoRepeat()) {
    return;
  }

  switch (event->key()) {
    case Qt::Key_A: controller_state |= (1u << 0u); break;
    case Qt::Key_S: controller_state |= (1u << 1u); break;
    case Qt::Key_Backspace: controller_state |= (1u << 2u); break;
    case Qt::Key_Return: controller_state |= (1u << 3u); break;
    case Qt::Key_Up: controller_state |= (1u << 4u); break;
    case Qt::Key_Down: controller_state |= (1u << 5u); break;
    case Qt::Key_Left: controller_state |= (1u << 6u); break;
    case Qt::Key_Right: controller_state |= (1u << 7u); break;

    case Qt::Key_F1: Emulator::save_snapshot(); break;
    case Qt::Key_F3: Emulator::load_snapshot(); break;
    case Qt::Key_Escape:
      running = !running;
      if (running) {
        auto title = fmt::format("{}", Emulator::title);
        setWindowTitle(title.c_str());

        fpsTimer      = std::chrono::steady_clock::now();
        elapsedFrames = 0;
      } else {
        auto title = fmt::format("{} | Paused", Emulator::title);
        setWindowTitle(title.c_str());
      }
      break;

    case Qt::Key_Plus: {
      volume = std::min(volume + 0.1, 1.0);
      break;
    }

    case Qt::Key_Minus: {
      volume = std::max(0.0, volume - 0.1);
      break;
    }
    default: event->ignore();
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
  if (event->isAutoRepeat()) {
    return;
  }

  switch (event->key()) {
    case Qt::Key_A: controller_state &= ~(1u << 0u); break;
    case Qt::Key_S: controller_state &= ~(1u << 1u); break;
    case Qt::Key_Backspace: controller_state &= ~(1u << 2u); break;
    case Qt::Key_Return: controller_state &= ~(1u << 3u); break;
    case Qt::Key_Up: controller_state &= ~(1u << 4u); break;
    case Qt::Key_Down: controller_state &= ~(1u << 5u); break;
    case Qt::Key_Left: controller_state &= ~(1u << 6u); break;
    case Qt::Key_Right: controller_state &= ~(1u << 7u); break;
    default: event->ignore();
  }
}

void MainWindow::resize(int scale)
{
  ui->video->setFixedSize(256 * scale, 240 * scale);
  adjustSize();
}
