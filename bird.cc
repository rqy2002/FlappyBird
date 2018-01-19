#include <cctype>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "game.h"
#include "win_manager.h"
const int kLines = 35;
const int kColumns = 120;
const int kLeftSideColumns = 20 + 1;
char tmp[200];
void WriteLeftSide(const Game &game, WinManager &left_side) {
  left_side.WriteColumn(kLeftSideColumns - 1, 0, -1, '|');
  left_side.WriteString(0, 1, "Help:");
  left_side.WriteString(0, 2, " Press <Space> to");
  left_side.WriteString(0, 3, "make the bird fly.");
  left_side.WriteString(0, 4, " Don't fall to the");
  left_side.WriteString(0, 5, "ground or hit pipes");
  // Empty line
  left_side.WriteString(0, 7, "Time:");
  sprintf(tmp, " %5.2lfs", game.get_time());
  left_side.WriteString(0, 8, tmp);
  left_side.WriteString(0, 9, "Fps:");
  sprintf(tmp, " %5.2lf", game.get_fps());
  left_side.WriteString(0, 10, tmp);
  // Empty line
  left_side.WriteString(0, 12, "Score:");
  sprintf(tmp, " %3d", game.get_score());
  left_side.WriteString(0, 13, tmp);
  left_side.WriteString(0, 14, "High Score:");
  sprintf(tmp, " %3d", game.get_high_score());
  left_side.WriteString(0, 15, tmp);
}
inline double GetTime() {
  return static_cast<double> (clock()) / CLOCKS_PER_SEC;
}
void InitMenu(WinManager &main_frame) {
  main_frame.WriteString(-1, kLines / 2 - 1, "Flappy bird (Simple)");
  main_frame.WriteString(-1, kLines / 2, "--By _rqy");
  main_frame.WriteString(-1, kLines / 2 + 1, "Press any key to start...");
  main_frame.UpdateScreen();
  WaitKey();
}
inline int toUpper(char c) {
  return islower(c) ? c + 'A' - 'a' : c;
}
int main() {
  sprintf(tmp, "mode con lines=%d cols=%d", kLines, kColumns);
  system(tmp);
  HideCursor();
  WinManager left_side(0, kLines, 0, kLeftSideColumns);
  WinManager main_frame(0, kLines, kLeftSideColumns, kColumns);
  Game game(kLines, kColumns - kLeftSideColumns);
  InitMenu(main_frame);
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
    }
    main_frame.WriteString(-1, kLines / 2 - 3, "-------------------------------");
    main_frame.WriteString(-1, kLines / 2 - 2, "|                             |");
    main_frame.WriteString(-1, kLines / 2 - 1, "|                             |");
    main_frame.WriteString(-1, kLines / 2 + 0, "|                             |");
    main_frame.WriteString(-1, kLines / 2 + 1, "|                             |");
    main_frame.WriteString(-1, kLines / 2 + 2, "|                             |");
    main_frame.WriteString(-1, kLines / 2 + 3, "-------------------------------");
    sprintf(tmp, "Your score: %d!", game.get_score());
    main_frame.WriteString(-1, kLines / 2 - 1, tmp);
    main_frame.WriteString(-1, kLines / 2 + 0, "Press R to restart.");
    main_frame.WriteString(-1, kLines / 2 + 1, "Press E to exit.");
    main_frame.UpdateScreen();
    int c;
    do c = toUpper(WaitKey()); while (c != 'E' && c != 'R');
    if (c == 'E') break;
    else continue;
  }
  return 0;
}
