#include "Recorder.hpp"

InputRecorder::InputRecorder(HIDControllerID controller)
    : mController(controller) {}

// this records ONE frame
InputInfos InputController::Record() {
  // initialize hid in case it wasn't ?
  // warning: you can initialize several time so you have to destroy it that
  // many times :pepeS:

  InputInfos recorded;

  // get the keys held for the designated controller at this time.
  hidScanInput();

  recorded.controller = this->mController;
  recorded.kHeld = hidKeysHeld(mController);
  // reading left & right joystick pos
  hidJoystickRead(&recorded.lpos, mController, JOYSTICK_LEFT);
  hidJoystickRead(&recorded.rpos, mController, JOYSTICK_RIGHT);

  return recorded;
}
