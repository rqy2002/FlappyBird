// A windows terminal manager
#ifndef FLAPPYBIRD_WIN_MANAGER_H_
#define FLAPPYBIRD_WIN_MANAGER_H_

#include <windows.h>
#include <map>

// A windows manager.
class WinManager{
 public:
  // WinManager(...):
  //  Create a manager with the block [left, right) * [up, down).
  WinManager(int up, int down, int left, int right);
  ~WinManager();
  // getHeight() and getWidth(): return the height/width of the block.
  int getHeight();
  int getWidth();
  // WriteCharacter(...):
  //  Write a character c on the position (x, y).
  //  Note that (x, y) is relatively position to (left_, right_).
  void WriteCharacter(int x, int y, char c);
  // WriteLine/WriteColumn(...):
  // Write a string filled by c from (x1, y) to (x2, y) (/ from (x, y1) to (x, y2))
  // x2/y2 can be -1, then it will be the width/height
  void WriteLine(int x1, int x2, int y, char c);
  void WriteColumn(int x, int y1, int y2, char c);
  // WriteString(...):
  // Write a string s from (x, y) to (x + strlen(s) - 1, y).
  // x can be -1, means that write in middle.
  void WriteString(int x, int y, const char *s);
  // UpdateScreen():
  // Update the map onto the screen.
  void UpdateScreen();
  // Clear():
  // Clear all contents in the block
  void Clear();
 private:
  int left_, right_, up_, down_;
  char **map_;
  std::map<std::pair<int, int>, char> changes_;
  HANDLE out_handle_;
};

// Hide the cursor.
void HideCursor();
// Wait for a key
int WaitKey();
// If a key is perssed then returns the key; or returns -1 otherwise
int GetKey();
// Wait until a time
void WaitUntil(double end);


#endif // FLAPPYBIRD_WINMANAGE_H_
