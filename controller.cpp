#include "controller.h"
#include <X11/Xlib.h>
#include <Input_Lite.h>
#include <thread>

#include <iostream>
#include <sstream>

using namespace std::chrono_literals;

void pressKey(SL::Input_Lite::KeyCodes key) {
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{true, key});
}

void unpressKey(SL::Input_Lite::KeyCodes key) {
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{false, key});
}

void unpressAll() {
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{false, SL::Input_Lite::KEY_A});
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{false, SL::Input_Lite::KEY_S});
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{false, SL::Input_Lite::KEY_J});
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{false, SL::Input_Lite::KEY_K});
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{false, SL::Input_Lite::KEY_L});
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{false, SL::Input_Lite::KEY_H});
}

void strum() {
  std::this_thread::sleep_for(2ms);
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{true, SL::Input_Lite::KEY_Up});
  std::this_thread::sleep_for(2ms);
  SL::Input_Lite::SendInput(SL::Input_Lite::KeyEvent{false, SL::Input_Lite::KEY_Up});
}
