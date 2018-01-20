#include "UI.h"
#include <cctype>
#include <cstring>
#include <algorithm>
#include "win_manager.h"

namespace {
inline int toUpper(char c) {
  return islower(c) ? c + 'A' - 'a' : c;
}
};

void DrawBox(WinManager &manager, int x0, int y0, int width, int height) {
  int x1 = x0 + width - 1, y1 = y0 + height - 1;
  manager.WriteColumn(x0, y0, y1, '|');
  manager.WriteColumn(x1, y0, y1, '|');
  manager.WriteLine(x0, x1, y0, '-');
  manager.WriteLine(x0, x1, y1, '-');
}

void Clear(WinManager &manager, int x0, int y0, int width, int height) {
  int x1 = x0 + width - 1, y1 = y0 + height - 1;
  for (int i = y0; i <= y1; ++i)
    manager.WriteLine(x0, x1, i, ' ');
}

int Menu(WinManager &manager, const char **messages, const char* keys) {
  using std::max;
  int n = strlen(keys), m = 0;
  for (int i = 0; i < n; ++i)
    m = max<int>(m, strlen(messages[i]) + (isspace(keys[i]) ? 0 : 3));
  int x0 = (manager.getWidth() - m) / 2, y0 = (manager.getHeight() - n) / 2;
  const int kMargin = 4, kVMargin = 2;
  Clear(manager, x0 - kMargin, y0 - kVMargin,
      m + kMargin * 2, n + kVMargin * 2);
  DrawBox(manager, x0 - kMargin, y0 - kVMargin,
      m + kMargin * 2, n + kVMargin * 2);
  for (int i = 0; i < n; ++i)
    manager.WriteString(x0 + (isspace(keys[i]) ? 0 : 3), y0 + i, messages[i]);
  int cur = 0, minc = 0, maxc = n - 1;
  while (isspace(keys[minc])) ++minc;
  while (isspace(keys[maxc])) --maxc;
  cur = minc;
  manager.WriteString(x0, y0 + cur, ">>");
  while (true) {
    manager.UpdateScreen();
    int c = WaitKey();
    int new_cur = cur;
    if (c == 224) {
      int c2 = WaitKey();
      if (c2 == 72 && cur > minc) // <Up>
        while (isspace(keys[--new_cur]));
      else if (c2 == 80 && cur < maxc) // <Down>
        while (isspace(keys[++new_cur]));
    } else if (c == ' ' || c == '\n' || c == '\r') {
      Clear(manager, x0 - kMargin, y0 - kVMargin,
          m + kMargin * 2, n + kVMargin * 2);
      manager.UpdateScreen();
      return cur;
    } else {
      for (int i = 0; i < n; ++i)
        if (toUpper(keys[i]) == toUpper(c)) {
          new_cur = i;
          break;
        }
    }
    manager.WriteString(x0, y0 + cur, "  ");
    manager.WriteString(x0, y0 + new_cur, ">>");
    cur = new_cur;
  }
}
