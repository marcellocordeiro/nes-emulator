#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <SDL.h>
#include <QCoreApplication>
#include <QKeyEvent>

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

  nes::Utility::FileManager::get().setup();
  nes::Utility::FileManager::get().set_rom(
      QCoreApplication::arguments().at(1).toStdString());
  nes::Emulator::get().power_on();

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
}

MainWindow::~MainWindow()
{
  SDL_PauseAudio(true);
  SDL_CloseAudio();

  SDL_Quit();
}

void MainWindow::run_frame()
{
  nes::Controller::get().update_state(0, controller_state);
  nes::CPU::get().run_frame();
  ui->video->update();

  if (nes::APU::get().samples_available(audio_buffer.size())) {
    auto sample_count =
        nes::APU::get().get_samples(audio_buffer.data(), audio_buffer.size());
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
