#include <algorithm>
#include <cctype>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "game.h"
#include "win_manager.h"
#include "UI.h"
const int kLines = 35;
const int kColumns = 120;
const int kLeftSideColumns = 20 + 1;
char tmp[200];
int high_score;

void WriteLeftSide(const Game &game, WinManager &left_side);
// Get current time.
inline double GetTime();
// Returns 2 if exit, 1 if play game
int MainMenu();
void Help(WinManager &manager);
// Returns 2 if exit, 1 if return back to main menu
int PlayGame();

inline int toUpper(char c) {
  return islower(c) ? c + 'A' - 'a' : c;
}

int main() {
  sprintf(tmp, "mode con lines=%d cols=%d", kLines, kColumns);
  system(tmp);
  HideCursor();
  int t = 0;
  while (1) {
    int res = t ? PlayGame() : MainMenu();
    if (res == 2) break;
    else t ^= 1;
  }
  return 0;
}

int PlayGame() {
  WinManager left_side(0, kLines, 0, kLeftSideColumns);
  WinManager main_frame(0, kLines, kLeftSideColumns, kColumns);
  Game game(kLines, kColumns - kLeftSideColumns);
  while (true) {
    main_frame.Clear();
    game.Init(GetTime());
    while (!game.CheckIfFail()) {
      double ti = GetTime();
      game.NextFrame(ti, GetKey(), main_frame);
      WriteLeftSide(game, left_side);
      left_side.UpdateScreen();
      main_frame.UpdateScreen();
      WaitUntil(ti + 0.020);
    };
    sprintf(tmp, "Your score: %d!", game.get_score());
    const char *menu[] = {tmp, "(R)estart.", "(B)ack to menu.", "(E)xit."};
    int res = Menu(main_frame, menu, " RBE");
    if (res == 1) continue;
    else {
      high_score = std::max(high_score, game.get_high_score());
      return res - 1;
    }
  }
}

void Help(WinManager &manager) {
  const char *help[] = {
    "Help:",
    " How to play:",
    "  Press <Space> to fly;",
    "  Don't fall to ground or hit pipes",
    "  Your score is the number of pipes you fly through.",
    "",
    " How to use menus:",
    "  Press <Up>/<Down> to switch options,",
    "  Or press one option's initial to switch to it",
    "  Press <Space>/<Enter> to confirm.",
    "",
    "(B)ack to menu"
  };
  Menu(manager, help, "           B");
}
int MainMenu() {
  WinManager manager(0, kLines, 0, kColumns);
  sprintf(tmp, "High score: %d", high_score);
  const char *menu[] = {
    "Flappy bird",
    "    --By _rqy",
    tmp,
    "(H)elp",
    "(S)tart",
    "(E)xit",
  };
  while (true) {
    int res = Menu(manager, menu, "   HSE");
    if (res == 3) Help(manager);
    else return res - 3;
  }
}

inline double GetTime() {
  return static_cast<double> (clock()) / CLOCKS_PER_SEC;
}

void WriteLeftSide(const Game &game, WinManager &left_side) {
  left_side.WriteColumn(kLeftSideColumns - 1, 0, -1, '|');
  int p = 0;
  ++p; left_side.WriteString(0, p, "Keys:");
  ++p; left_side.WriteString(0, p, " <Space>:");
  ++p; left_side.WriteString(0, p, "  make the bird fly.");
  ++p; left_side.WriteString(0, p, " P:");
  ++p; left_side.WriteString(0, p, "  pause or continue.");
  ++p; // Empty line
  ++p; left_side.WriteString(0, p, " Don't fall to the");
  ++p; left_side.WriteString(0, p, "ground or hit pipes");
  ++p; // Empty line
  ++p; left_side.WriteString(0, p, "Time:");
  sprintf(tmp, " %5.2lfs", game.get_time());
  ++p; left_side.WriteString(0, p, tmp);
  ++p; left_side.WriteString(0, p, "Fps:");
  sprintf(tmp, " %5.2lf", game.get_fps());
  ++p; left_side.WriteString(0, p, tmp);
  ++p; // Empty line
  ++p; left_side.WriteString(0, p, "Score:");
  sprintf(tmp, " %3d", game.get_score());
  ++p; left_side.WriteString(0, p, tmp);
  ++p; left_side.WriteString(0, p, "High Score:");
  sprintf(tmp, " %3d", std::max(high_score, game.get_high_score()));
  ++p; left_side.WriteString(0, p, tmp);
}
