#ifndef FLAPPYBIRD_UI_H_
#define FLAPPYBIRD_UI_H_

class WinManager;

// DrawBox(...):
//  Draw a box
//   width
//  -------
//  |     | height
//  -------
//  topleft on (x0, y0).
void DrawBox(WinManager &manager, int x0, int y0, int width, int height);
// ClearRect(...):
//  Clear all contents in rectangle [x0, x0 + width) * [y0, y0 + height)
void DrawBox(WinManager &manager, int x0, int y0, int width, int height);
// Menu(...):
//  Create a menu at the middle of the block of manager.
//  `messages' is messages to show;
//  keys[i] is the key of messages[i](can be alphas, digits), if no, ' '.
//  Note: keys must be distinct.
//  Returns the order of the term which is confirmed.
//  For example,
//  messages is {"(F)oo", "Haha", "(B)ar"}, keys is "F B",
//  then it will show
//  ------------
//  | >> (F)oo |
//  | Haha     |
//  |    (B)ar |
//  ------------
//  and user can use <Up>/<Down> or F/B to switch, <Enter>/<Space> to confirm.
//  If user press <Up> at first, nothing will happens.
//  If user press <Down> then, it will skip "Haha" and point to "(S)ub".
//  If user press <Enter>,
//    it will clean the menu from screen, and return 1.
// Note 2: if there is something covered by the menu, it will disappear.
int Menu(WinManager &manager, const char **messages, const char* keys);

#endif // FlAPPYBIRD_UI_H_
