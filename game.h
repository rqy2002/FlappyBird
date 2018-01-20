// The game state, and some calculation
// Note:
//  All position has unit `grid';
//  x axis from left to right, y axis from top to bottom.
//  All velocity has unit `grid per second'.
//  Acceleration has unit `grid per square second'.
#ifndef FLAPPYBIRD_GAME_H_
#define FLAPPYBIRD_GAME_H_
class WinManager;


// A pipe with position x and the break (H, D)
struct Pipe{
  double x;
  int H, D;
};

class Game{
 public:
  Game(int height, int width);
  ~Game();
  void Init(double start_time);
  // Get next frame.
  //  `key' is the key which user presses; if no key, `key' = -1.
  void NextFrame(double current_time, int key, WinManager &manager);
  int get_score() const;
  int get_high_score() const;
  double get_time() const;
  double get_fps() const;
  bool CheckIfFail() const;
 private:
  void NewPipe();
  bool HitPipe(const Pipe &p) const;
  double bird_y_, speed_y_;
  Pipe *pipes_;
  int frame_count_;
  double last10_time_;
  double start_time_, last_time_, fps_;
  int height_, width_, score_, high_score_, pipe_count_;
  bool paused_;
};

#endif // FLAPPYBIRD_GAME_H_
