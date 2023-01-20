#include "controller.h"
#include <X11/Xlib.h>
#include <Input_Lite.h>

void pressKey(SL::Input_Lite::KeyCodes key) {
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{true, key});
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{false, key});
}
