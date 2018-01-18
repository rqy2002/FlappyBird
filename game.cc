#include "game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "win_manager.h"

// Space between adjective two pipes
const int kPipeSpace = 30;
// Speed along the x axis
const double kSpeedX = 12.0;
// Range of length of break
const int kLengthL = 8, kLengthR = 10;
// Speed along the y axis when user press the key
const double kUpSpeedY = -15.0;
// Griviational Acceleration
const double kGrivAcc = 35.0;
// Abscissa of the bird
const double kBirdX = 7.0;

namespace {
static inline int toInt(double x) {
  return static_cast<int> (x + 0.5);
}
void ClearBird(WinManager &manager, double bird_y) {
  int x = toInt(kBirdX), y = toInt(bird_y);
  manager.WriteCharacter(x, y - 1, ' ');
  manager.WriteLine(x - 1, x + 1, y, ' ');
  manager.WriteCharacter(x, y + 1, ' ');
}

void DrawBird(WinManager &manager, double bird_y) {
  int x = toInt(kBirdX), y = toInt(bird_y);
  manager.WriteCharacter(x, y - 1, 'v');
  manager.WriteLine(x - 1, x + 1, y, '>');
  manager.WriteCharacter(x, y + 1, '^');
}

void ClearPipe(WinManager &manager, const Pipe &pipe) {
  int x = toInt(pipe.x);
  manager.WriteColumn(x, 0, pipe.H, ' ');
  manager.WriteColumn(x + 1, 0, pipe.H, ' ');
  manager.WriteColumn(x, pipe.D, -1, ' ');
  manager.WriteColumn(x + 1, pipe.D, -1, ' ');
}

void DrawPipe(WinManager &manager, const Pipe &pipe) {
  int x = toInt(pipe.x);
  manager.WriteColumn(x, 0, pipe.H - 1, '|');
  manager.WriteColumn(x + 1, 0, pipe.H - 1, '|');
  manager.WriteColumn(x, pipe.D + 1, -1, '|');
  manager.WriteColumn(x + 1, pipe.D + 1, -1, '|');
  manager.WriteLine(x, x + 1, pipe.H, '-');
  manager.WriteLine(x, x + 1, pipe.D, '-');
}
};

Game::Game(int height, int width)
    : height_(height), width_(width) {
  pipes_ = new Pipe[width / kPipeSpace + 5];
}

Game::~Game() {
  delete pipes_;
}

void Game::Init(double start_time) {
  srand(time(NULL));
  last10_time_ = start_time_ = last_time_ = start_time;
  fps_ = speed_y_ = .0;
  frame_count_ = score_ = pipe_count_ = 0;
  bird_y_ = height_ * 0.15;
}

void Game::NextFrame(double current_time, int key, WinManager &manager) {
  double time_break = current_time - last_time_;
  last_time_ = current_time;
  ++frame_count_;
  if (frame_count_ == 10) {
    fps_ = 10.0 / (current_time - last10_time_);
    last10_time_ = current_time;
    frame_count_ = 0;
  }
  ClearBird(manager, bird_y_);
  double last_speed_y = speed_y_;
  if (key == ' ')
    last_speed_y = speed_y_ = kUpSpeedY;
  else
    speed_y_ += kGrivAcc * time_break;
  bird_y_ += 0.5 * (last_speed_y + speed_y_) * time_break;
  bird_y_ = std::max(bird_y_, 1.0);
  bird_y_ = std::min(bird_y_, height_ - 0.7);
  int new_pipe_count = 0;
  for (int i = 0; i < pipe_count_; ++i) {
    ClearPipe(manager, pipes_[i]);
    pipes_[i].x -= kSpeedX * time_break;
    if (pipes_[i].x > 0) {
      DrawPipe(manager, pipes_[i]);
      pipes_[new_pipe_count++] = pipes_[i];
    } else {
      ++score_;
    }
  }
  pipe_count_ = new_pipe_count;
  if (width_ - pipes_[pipe_count_ - 1].x > kPipeSpace) {
    NewPipe();
    DrawPipe(manager, pipes_[pipe_count_ - 1]);
  }
  DrawBird(manager, bird_y_);
}

int Game::get_score() const { return score_; }
double Game::get_time() const { return last_time_ - start_time_; }
double Game::get_fps() const { return fps_; }

bool Game::CheckIfFail() const {
  if (bird_y_ + 1.0 >= height_) return true;
  for (int i = 0; i < pipe_count_; ++i)
    if (HitPipe(pipes_[i])) return true;
  return false;
}

void Game::NewPipe() {
  Pipe &new_pipe = pipes_[pipe_count_++];
  new_pipe.x = width_ - 0.5;
  int h = rand() % (kLengthR - kLengthL + 1) + kLengthL;
  new_pipe.H = rand() % (height_ - h + 1 - 6) + 3;
  new_pipe.D = new_pipe.H + h;
}

bool Game::HitPipe(const Pipe &p) const {
  int dx = toInt(kBirdX) - toInt(p.x);
  dx = std::min(std::abs(dx), std::abs(dx - 1));
  if (dx > 2)
    return false;
  else if (dx == 2)
    return bird_y_ <= p.H - 0.5 || bird_y_ >= p.D + 0.5;
  else if (dx == 1)
    return bird_y_ <= p.H + 0.5 || bird_y_ >= p.D - 0.5;
  else
    return bird_y_ <= p.H + 1.5 || bird_y_ >= p.D - 1.5;
}
