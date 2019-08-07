#include "Recorder.hpp"

InputRecorder::InputRecorder(HidControllerID controller)
    : mController(controller)
{
    Logger logger;
    logger->trace("Created recorder\r\n");
    logger->trace("Initializing Six Axis Sensor");

    /*
        logger->info("Initializing Six Axis Sensor Handle: type joycon\r\n");
        hidGetSixAxisSensorHandles(&mHandles[0], 2, controller,
       TYPE_JOYCON_PAIR); logger->info("Initializing Six Axis Sensor Handle:
       type procontroller\r\n"); hidGetSixAxisSensorHandles(&mHandles[2], 1,
       controller, TYPE_PROCONTROLLER); logger->info( "Initializing Six Axis
       Sensor Handle: type controller handheld\r\n");
        hidGetSixAxisSensorHandles(&mHandles[3], 1, CONTROLLER_HANDHELD,
                                   TYPE_HANDHELD);
        logger->info("Starting six axis handle 0\r\n");
        hidStartSixAxisSensor(mHandles[0]);

        logger->info("Starting six axis handle 1\r\n");
        hidStartSixAxisSensor(mHandles[1]);

        logger->info("Starting six axis handle 2\r\n");
        hidStartSixAxisSensor(mHandles[2]);

        logger->info("Starting six axis handle 3\r\n");
        hidStartSixAxisSensor(mHandles[2]);
    */

    // if this is a joycon pair we need to initialize the two thing in the
    // array? i think..

    if (hidGetControllerType(this->mController) == TYPE_JOYCON_PAIR) {
        logger->trace("Getting SixAxisSensor handle for joycon pair\r\n");
        hidGetSixAxisSensorHandles(&mHandles[0], 2, this->mController,
                                   TYPE_JOYCON_PAIR);

        logger->trace("Starting sensor...\r\n");
        hidStartSixAxisSensor(mHandles[1]);
    } else {
        logger->trace(
            "Getting SixAxisSensor handle for whatever the fuck this is\r\n");
        hidGetSixAxisSensorHandles(&mHandles[0], 1, this->mController,
                                   hidGetControllerType(this->mController));

        logger->trace("Starting sensor...\r\n");
        hidStartSixAxisSensor(mHandles[0]);
    }
}

// this records ONE frame
InputInfos InputRecorder::Record()
{
    // initialize hid in case it wasn't ?
    // warning: you can initialize several time so you have to destroy it that
    // many times :pepeS:
    Logger test;
    InputInfos recorded;

    // get the keys held for the designated controller at this time.
    hidScanInput();

    recorded.controller = this->mController;
    recorded.kHeld = hidKeysHeld(mController);
    // reading left & right joystick pos
    hidJoystickRead(&recorded.lPos, mController, JOYSTICK_LEFT);
    hidJoystickRead(&recorded.rPos, mController, JOYSTICK_RIGHT);
    // trying to read motion controls i guess
    hidSixAxisSensorValuesRead(&recorded.motions, mController, 1);

    return recorded;
}

InputRecorder::~InputRecorder()
{
    Logger logger;
    logger->info("Stopping Six Axis Sensor\r\n");
    hidStopSixAxisSensor(mHandles[0]);
    hidStopSixAxisSensor(mHandles[1]);
    logger->trace("Destroying recorder\r\n");
}