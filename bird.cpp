#include <algorithm>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <windows.h>

const int Lines = 35;
const int Columns = 120;
const int LeftLineColumn = 20;
HANDLE hOut;
DWORD dword;
char tmp[1000];
int cur_time, start_time, last_time;
int score;

void DrawBird(double xx, double yy) {
  int x = (int)(xx + 0.5);
  int y = (int)(yy + 0.5);
  static DWORD dword;
  WriteConsoleOutputCharacter(hOut, "v", 1, (COORD){x, y - 1}, &dword);
  WriteConsoleOutputCharacter(hOut, ">>>", 3, (COORD){x - 1, y}, &dword);
  WriteConsoleOutputCharacter(hOut, "^", 1, (COORD){x, y + 1}, &dword);
}
void ClearBird(double xx, double yy) {
  int x = (int)(xx + 0.5);
  int y = (int)(yy + 0.5);
  WriteConsoleOutputCharacter(hOut, " ", 1, (COORD){x, y - 1}, &dword);
  WriteConsoleOutputCharacter(hOut, "   ", 3, (COORD){x - 1, y}, &dword);
  WriteConsoleOutputCharacter(hOut, " ", 1, (COORD){x, y + 1}, &dword);
}

/* Write at Middle/Left */
inline void WriteMiddle(const char* str, int y) {
  int l = strlen(str);
  WriteConsoleOutputCharacter(hOut, str, l, (COORD){(Columns - l) / 2, y}, &dword);
}
inline void WriteLeft(const char* str, int y) {
  int l = strlen(str);
  WriteConsoleOutputCharacter(hOut, str, l, (COORD){1, y}, &dword);
}

void InitLeftSide() {
  for (int i = 0; i < Lines; ++i)
    WriteConsoleOutputCharacter(hOut, "|", 2, (COORD){LeftLineColumn, i}, &dword);
  int p = 0;
  ++p; WriteLeft("Help:", p);
  ++p; WriteLeft(" Press <Space> to", p);
  ++p; WriteLeft("make the bird fly.", p);
  ++p; WriteLeft(" Don't fall to the", p);
  ++p; WriteLeft("ground or hit pipes", p);
  ++p; // Empty line
  ++p; WriteLeft("Time:", p);
  ++p; // Time
  ++p; WriteLeft("Fps:", p);
  ++p; // Fps
  ++p; // Empty line
  ++p; WriteLeft("Score:", p);
  ++p; // Score
}
void DrawLeftSide() {
  int p = 0;
  ++p; // WriteLeft("Help:", p);
  ++p; // WriteLeft(" Press <Space> to", p);
  ++p; // WriteLeft("make the bird fly.", p);
  ++p; // WriteLeft(" Don't fall to the", p);
  ++p; // WriteLeft("ground or hit pipes", p);
  ++p; // Empty line
  ++p; // WriteLeft("Time:", p);
  sprintf(tmp, " %5.2lfs", (double)(cur_time - start_time) / CLOCKS_PER_SEC);
  ++p; WriteLeft(tmp, p);
  ++p; // WriteLeft("Fps:", p);
  sprintf(tmp, " %5.2lf", CLOCKS_PER_SEC / (double)(cur_time - last_time));
  ++p; WriteLeft(tmp, p);
  ++p; // Empty line
  ++p; // WriteLeft("Score:", p);
  sprintf(tmp, " %3d", score);
  ++p; WriteLeft(tmp, p);
}

struct Pipe{
  int H, D; // A Pipe, [x, x + 1] * ([0, H] and [D, Lines))
  double x;
};

inline int RandRange(int l, int r) { return rand() % (r - l + 1) + l; }
inline Pipe RandomPipe(int x) {
  int br = RandRange(8, 10);
  int h = RandRange(3, Lines - br - 3);
  return (Pipe){h, h + br, (double)x};
}

inline bool CheckPipe(int x, double y, const Pipe &p) {
  int px = (int)(p.x + 0.5);
  int dx = std::min(std::abs(x - px), std::abs(x - px - 1));
  if (dx > 2)
    return true;
  else if (dx == 2)
    return y > p.H - 0.5 && y < p.D + 0.5;
  else if (dx == 1)
    return y > p.H + 0.5 && y < p.D - 0.5;
  else
    return y > p.H + 1.5 && y < p.D - 1.5;
}

void ClearPipe(const Pipe &p) {
  int px = (int)(p.x + 0.5);
  if (px != LeftLineColumn && px + 1 < Columns) {
    for (int i = 0; i < p.H; ++i)
      WriteConsoleOutputCharacter(hOut, "  ", 2, (COORD){px, i}, &dword);
    for (int i = p.D + 1; i < Lines; ++i)
      WriteConsoleOutputCharacter(hOut, "  ", 2, (COORD){px, i}, &dword);
    WriteConsoleOutputCharacter(hOut, "  ", 2, (COORD){px + 1, p.H}, &dword);
    WriteConsoleOutputCharacter(hOut, "  ", 2, (COORD){px + 1, p.D}, &dword);
  } else {
    int x = px == LeftLineColumn ? px + 1 : px;
    for (int i = 0; i < p.H; ++i)
      WriteConsoleOutputCharacter(hOut, " ", 1, (COORD){x, i}, &dword);
    for (int i = p.D + 1; i < Lines; ++i)
      WriteConsoleOutputCharacter(hOut, " ", 1, (COORD){x, i}, &dword);
    WriteConsoleOutputCharacter(hOut, " ", 1, (COORD){x, p.H}, &dword);
    WriteConsoleOutputCharacter(hOut, " ", 1, (COORD){x, p.D}, &dword);
  }
}
void DrawPipe(const Pipe &p) {
  int px = (int)(p.x + 0.5);
  if (px < LeftLineColumn) return;
  if (px != LeftLineColumn && px + 1 < Columns) {
    for (int i = 0; i < p.H; ++i)
      WriteConsoleOutputCharacter(hOut, "||", 2, (COORD){px, i}, &dword);
    for (int i = p.D + 1; i < Lines; ++i)
      WriteConsoleOutputCharacter(hOut, "||", 2, (COORD){px, i}, &dword);
    WriteConsoleOutputCharacter(hOut, "--", 2, (COORD){px, p.H}, &dword);
    WriteConsoleOutputCharacter(hOut, "--", 2, (COORD){px, p.D}, &dword);
  } else {
    int x = px == LeftLineColumn ? px + 1 : px;
    for (int i = 0; i < p.H; ++i)
      WriteConsoleOutputCharacter(hOut, "|", 1, (COORD){x, i}, &dword);
    for (int i = p.D + 1; i < Lines; ++i)
      WriteConsoleOutputCharacter(hOut, "|", 1, (COORD){x, i}, &dword);
    WriteConsoleOutputCharacter(hOut, "-", 1, (COORD){x, p.H}, &dword);
    WriteConsoleOutputCharacter(hOut, "-", 1, (COORD){x, p.D}, &dword);
  }
}

inline void HideCursor() {
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo(hOut,&cci);
  cci.bVisible = FALSE;
  SetConsoleCursorInfo(hOut,&cci);
}
void Init() {
  srand(time(NULL));
  sprintf(tmp, "mode con lines=%d cols=%d", Lines, Columns);
  system(tmp);
  hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  HideCursor();
  InitLeftSide();
  WriteMiddle("Flappy bird (Simple)", Lines / 2 - 1);
  WriteMiddle("--By _rqy", Lines / 2);
  WriteMiddle("Press any key to start...", Lines / 2 + 1);
  _getch();
  system("cls");
}

Pipe pipes[10];
int main() {
  Init();
  InitLeftSide();
  last_time = start_time = clock();
  score = 0;
  const double g = 35; // Gravitational acceleration, grid / s^2
  const double vx = 12.0;
  const double x = 27.0;
  double y = 5.0, vy = 0.0;
  int pipe_n = 0;
  pipes[pipe_n++] = RandomPipe(Columns - 1);
  while (1) {
    if (kbhit()) {
      char c = _getch();
      if (c == 32) vy = -15.0;
    }
    cur_time = clock();
    double time_break = (double)(cur_time - last_time) / CLOCKS_PER_SEC;
    ClearBird(x, y);
    y = std::min(vy * time_break + y, 33.0);
    vy += g * time_break;
    if (y < 1 - 1e-6) { y = 1; vy = .0; }
    DrawBird(x, y);
    for (int i = 0; i < pipe_n; ++i) {
      double px = pipes[i].x, px2 = pipes[i].x - vx * time_break;
      if ((int)(px + 0.5) != (int)(px2 + 0.5)) {
        ClearPipe(pipes[i]);
        pipes[i].x = px2;
        DrawPipe(pipes[i]);
      } else {
        pipes[i].x = px2;
      }
    }
    if (Columns - pipes[pipe_n - 1].x > 30)
      DrawPipe(pipes[pipe_n++] = RandomPipe(Columns - 1));
    int j = 0;
    bool failed = false;
    for (int i = 0; i < pipe_n; ++i) {
      failed = failed || !CheckPipe((int)(x + 0.5), y, pipes[i]);
      if ((int)(pipes[i].x + 0.5) >= LeftLineColumn)
        pipes[j++] = pipes[i];
      else
        ++score;
    }
    pipe_n = j;
    DrawLeftSide();
    if (y >= 33.0 - 1e-6 || failed) {
      DrawBird(x, y);
      WriteMiddle("-------------------------------", Lines / 2 - 2);
      WriteMiddle("|                             |", Lines / 2 - 1);
      WriteMiddle("|                             |", Lines / 2);
      WriteMiddle("|                             |", Lines / 2 + 1);
      WriteMiddle("|                             |", Lines / 2 + 2);
      WriteMiddle("-------------------------------", Lines / 2 + 3);
      sprintf(tmp, "Your score: %d!", score);
      WriteMiddle(tmp, Lines / 2);
      WriteMiddle("Press any key to exit...", Lines / 2 + 1);
      _getch();
      return 0;
    }
    last_time = cur_time;
    Sleep(40);
  }
  return 0;
}
