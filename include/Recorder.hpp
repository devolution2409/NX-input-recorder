#pragma once

#include <switch.h>
// https://switchbrew.github.io/libnx/hid_8h.html
/*   CONTROLLER_PLAYER_1 = 0,
  CONTROLLER_PLAYER_2 = 1,
  CONTROLLER_PLAYER_3 = 2,
  CONTROLLER_PLAYER_4 = 3,
  CONTROLLER_PLAYER_5 = 4,
  CONTROLLER_PLAYER_6 = 5,
  CONTROLLER_PLAYER_7 = 6,
  CONTROLLER_PLAYER_8 = 7,
  CONTROLLER_HANDHELD = 8,
  CONTROLLER_UNKNOWN = 9,
  CONTROLLER_P1_AUTO = 10  */

struct InputInfos {
  HidControllerID controller;
  u64 kHeld;
  JoystickPosition lPos;
  JoystickPosition rPos;
};

class InputRecorder {

public:
  // Specify which controller we want to record
  InputRecorder(HidControllerID controller);
  ~InputRecorder();
  InputInfos Record();

private:
  HidControllerID mController; // which controller are we recording
  InputRecorder();
};
