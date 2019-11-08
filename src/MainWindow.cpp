#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <SDL.h>
#include <QCoreApplication>
#include <QKeyEvent>
#include <algorithm>

#include <nes/Emulator.h>

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

  Emulator::load(QCoreApplication::arguments().at(1).toStdString());
  Emulator::power_on();
  Emulator::volume(0.1);

  sound_queue = std::make_unique<Sound_Queue>();
  sound_queue->init();

  SDL_AudioSpec want;
  SDL_zero(want);
  want.freq     = 44100;
  want.format   = AUDIO_S16SYS;
  want.channels = 1;
  want.samples  = Sound_Queue::buf_size;
  want.userdata = sound_queue.get();
  want.callback = [](void* user_data, Uint8* out, int count) {
    static_cast<Sound_Queue*>(user_data)->fill_buffer(out, count);
  };

  SDL_Init(SDL_INIT_AUDIO);

  SDL_OpenAudio(&want, nullptr);
  SDL_PauseAudio(false);

  connect(timer.get(), &QTimer::timeout, this, &MainWindow::run_frame);
  timer->start(1000.0f / framerate);

  running = true;
}

MainWindow::~MainWindow()
{
  SDL_PauseAudio(true);
  SDL_CloseAudio();

  SDL_Quit();
}

void MainWindow::run_frame()
{
  /*auto fps =
            elapsed_frames / duration<double>(fps_timer.elapsed_time()).count();
        auto title = fmt::format("{} | {:5.2f}fps", Emulator::title, fps);
        SDL_SetWindowTitle(window, title.c_str());
      } else {
        SDL_SetWindowTitle(window, "nes-emulator | Paused");*/
  if (!running) return;

  Emulator::update_controller_state(0, controller_state);
  Emulator::run_frame();
  ui->video->update();

  if (Emulator::samples_available(audio_buffer.size())) {
    auto sample_count = Emulator::get_audio_samples(audio_buffer);
    sound_queue->write(audio_buffer.data(), sample_count);
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
    case Qt::Key_Escape: running = !running; break;

    case Qt::Key_Plus: {
      volume = std::min(volume + 0.1, 1.0);
      Emulator::volume(volume);
      break;
    }

    case Qt::Key_Minus: {
      volume = std::max(0.0, volume - 0.1);
      Emulator::volume(volume);
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
