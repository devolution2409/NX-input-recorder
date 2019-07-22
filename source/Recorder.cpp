#include "Recorder.hpp"

InputRecorder::InputRecorder(HidControllerID controller)
    : mController(controller)
{

    Logger test;
    test->start();

    test->trace("Input Recorder created for controller %i", controller);
    std::stringstream wtf;
    wtf << "Adress of my shit here " << test.GetAdress();
    test->trace(wtf.str().c_str());
}

// this records ONE frame
InputInfos InputRecorder::Record()
{
    // initialize hid in case it wasn't ?
    // warning: you can initialize several time so you have to destroy it that
    // many times :pepeS:

    InputInfos recorded;

    // get the keys held for the designated controller at this time.
    hidScanInput();

    recorded.controller = this->mController;
    recorded.kHeld = hidKeysHeld(mController);
    // reading left & right joystick pos
    hidJoystickRead(&recorded.lPos, mController, JOYSTICK_LEFT);
    hidJoystickRead(&recorded.rPos, mController, JOYSTICK_RIGHT);

    return recorded;
}

InputRecorder::~InputRecorder() {}