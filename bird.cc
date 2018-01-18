#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "game.h"
#include "win_manager.h"
const int kLines = 35;
const int kColumns = 120;
const int kLeftSideColumns = 20 + 1;
char tmp[200];
void InitLeftSide(WinManager &left_side) {
  left_side.WriteColumn(kLeftSideColumns - 1, 0, -1, '|');
  left_side.WriteString(0, 1, "Help:");
  left_side.WriteString(0, 2, " Press <Space> to");
  left_side.WriteString(0, 3, "make the bird fly.");
  left_side.WriteString(0, 4, " Don't fall to the");
  left_side.WriteString(0, 5, "ground or hit pipes");
  // Empty line
  left_side.WriteString(0, 7, "Time:");
  // Time
  left_side.WriteString(0, 9, "Fps:");
  // Fps
  // Empty line
  left_side.WriteString(0, 12, "Score:");
  // Score
}
void WriteLeftSide(const Game &game, WinManager &left_side) {
  sprintf(tmp, " %5.2lfs", game.get_time());
  left_side.WriteString(0, 8, tmp);
  sprintf(tmp, " %5.2lf", game.get_fps());
  left_side.WriteString(0, 10, tmp);
  sprintf(tmp, " %3d", game.get_score());
  left_side.WriteString(0, 13, tmp);
}
double GetTime() {
  return static_cast<double> (clock()) / CLOCKS_PER_SEC;
}
inline void HideCursor() {
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo(hOut,&cci);
  cci.bVisible = FALSE;
  SetConsoleCursorInfo(hOut,&cci);
}
void InitMenu(WinManager &main_frame) {
  main_frame.WriteString(-1, kLines / 2 - 1, "Flappy bird (Simple)");
  main_frame.WriteString(-1, kLines / 2, "--By _rqy");
  main_frame.WriteString(-1, kLines / 2 + 1, "Press any key to start...");
  main_frame.UpdateScreen();
  _getch();
  main_frame.WriteString(-1, kLines / 2 - 1, "                    ");
  main_frame.WriteString(-1, kLines / 2, "         ");
  main_frame.WriteString(-1, kLines / 2 + 1, "                         ");
}
int main() {
  sprintf(tmp, "mode con lines=%d cols=%d", kLines, kColumns);
  system(tmp);
  HideCursor();
  WinManager left_side(0, kLines, 0, kLeftSideColumns);
  WinManager main_frame(0, kLines, kLeftSideColumns, kColumns);
  Game game(kLines, kColumns - kLeftSideColumns);
  InitLeftSide(left_side);
  left_side.UpdateScreen();
  InitMenu(main_frame);
  game.Init(GetTime());
  while (!game.CheckIfFail()) {
    game.NextFrame(GetTime(), kbhit() ? _getch() : -1, main_frame);
    WriteLeftSide(game, left_side);
    left_side.UpdateScreen();
    main_frame.UpdateScreen();
    Sleep(30);
  }
  main_frame.WriteString(-1, kLines / 2 - 2, "-------------------------------");
  main_frame.WriteString(-1, kLines / 2 - 1, "|                             |");
  main_frame.WriteString(-1, kLines / 2, "|                             |");
  main_frame.WriteString(-1, kLines / 2 + 1, "|                             |");
  main_frame.WriteString(-1, kLines / 2 + 2, "|                             |");
  main_frame.WriteString(-1, kLines / 2 + 3, "-------------------------------");
  sprintf(tmp, "Your score: %d!", game.get_score());
  main_frame.WriteString(-1, kLines / 2, tmp);
  main_frame.WriteString(-1, kLines / 2 + 1, "Press any key to exit...");
  main_frame.UpdateScreen();
  _getch();
  return 0;
}
