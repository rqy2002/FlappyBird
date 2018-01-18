#include "win_manager.h"
#include <conio.h>
#include <windows.h>
#include <cstring>
#include <map>

WinManager::WinManager(int up, int down, int left, int right)
    : left_(left), right_(right), up_(up), down_(down) {
  out_handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
  int h = getHeight(), w = getWidth();
  map_ = new char *[h];
  for (int i = 0; i < h; ++i) map_[i] = new char [w];
}

WinManager::~WinManager() {
  delete map_;
}
int WinManager::getHeight() { return down_ - up_; }

int WinManager::getWidth() { return right_ - left_; }

void WinManager::WriteCharacter(int x, int y, char c) {
  if (x >= 0 && y >= 0 && x < getWidth() && y < getHeight())
    changes_[std::make_pair(x, y)] = c;
}

void WinManager::WriteLine(int x1, int x2, int y, char c) {
  if (x2 == -1) x2 = getWidth();
  for (int x = x1; x <= x2; ++x)
    WriteCharacter(x, y, c);
}

void WinManager::WriteColumn(int x, int y1, int y2, char c) {
  if (y2 == -1) y2 = getHeight();
  for (int y = y1; y <= y2; ++y)
    WriteCharacter(x, y, c);
}

void WinManager::WriteString(int x, int y, const char *s) {
  if (x == -1) x = (getWidth() - strlen(s)) / 2;
  while (*s) WriteCharacter(x++, y, *(s++));
}

void WinManager::UpdateScreen() {
  DWORD dword;
  for (std::map<std::pair<int, int>, char>::iterator it = changes_.begin();
   it != changes_.end(); ++it) {
    int x = it->first.first, y = it->first.second;
    char c = it->second;
    if (map_[y][x] != c) {
      map_[y][x] = c;
      WriteConsoleOutputCharacter(out_handle_, &c, 1,
                                  (COORD){left_ + x, up_ + y},
                                  &dword);
    }
  }
  changes_.clear();
}
