// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
// this should probably be cstring but whatever
//#include <string.h>
// Include the main libnx system header, for Switch development
#include <switch.h>

#include "Recorder.hpp"
#include "helpers/Logger.hpp"
#include "helpers/RecordWriter.hpp"
#include "helpers/SystemHelper.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>
// Adjust size as needed.
#define INNER_HEAP_SIZE 0x80000

// add wrapper to logger for the init functions i guess

extern "C" {
// Sysmodules should not use applet*.
u32 __nx_applet_type = AppletType_None;
size_t nx_inner_heap_size = INNER_HEAP_SIZE;
char nx_inner_heap[INNER_HEAP_SIZE];

void __libnx_initheap(void)
{
    void *addr = nx_inner_heap;
    size_t size = nx_inner_heap_size;

    // Newlib
    extern char *fake_heap_start;
    extern char *fake_heap_end;

    fake_heap_start = (char *)addr;
    fake_heap_end = (char *)addr + size;
}

// Init/exit services, update as needed.
void __attribute__((weak)) __appInit(void)
{
    Result rc;

    // Initialize default services.
    rc = smInitialize();
    if (R_FAILED(rc))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));

    // Enable this if you want to use HID.
    rc = hidInitialize();
    if (R_FAILED(rc))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID));

    // Enable this if you want to use time.
    /*rc = timeInitialize();
      if (R_FAILED(rc))
      fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_Time));

      __libnx_init_time();*/

    rc = fsInitialize();
    if (R_FAILED(rc))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));

    // init vsync events?
    rc = viInitialize(ViServiceType_System);
    if (R_FAILED(rc))
        fatalSimple(rc);

    // need this to get pId
    rc = pmdmntInitialize();
    if (R_FAILED(rc)) {
        fatalSimple(rc);
    }

    // need this to get applicationid
    rc = pminfoInitialize();
    if (R_FAILED(rc))
        fatalSimple(rc);

    // setting hos version because apparently it changes some functions
    rc = setsysInitialize();
    if (R_SUCCEEDED(rc)) {
        SetSysFirmwareVersion fw;
        rc = setsysGetFirmwareVersion(&fw);
        if (R_SUCCEEDED(rc))
            hosversionSet(MAKEHOSVERSION(fw.major, fw.minor, fw.micro));
        setsysExit();
    }
    rc = nsInitialize();
    if (R_FAILED(rc))
        fatalSimple(rc);

    fsdevMountSdmc();
}

void __attribute__((weak)) userAppExit(void);
void __attribute__((weak)) __appExit(void)
{
    // Cleanup default services.
    fsdevUnmountAll();
    fsExit();

    // timeExit();//Enable this if you want to use time.
    hidExit(); // Enable this if you want to use HID.
    smExit();
    pmdmntExit();
    nsExit();
}

} // end extern C

// Main program entrypoint
int main(int argc, char *argv[])
{
    Logger logger;
    logger->start();

    //  Logger::getInstance()->start();

    bool record = false;
    Event vsync_event;

    Result rc;
    ViDisplay disp;

    rc = viOpenDefaultDisplay(&disp);
    if (R_FAILED(rc))
        fatalSimple(rc);
    rc = viGetDisplayVsyncEvent(&disp, &vsync_event);
    if (R_FAILED(rc))
        fatalSimple(rc);

    std::vector<InputRecorder *> recorders;
    Helper::RecordWriter *writer =
        nullptr; // silence this annoying maybe-unitialized

    while (true) {
        // wait on screen refresh i guess
        // uh whats a good timeout kev
        // fs << "waiting for next frame ResidentSleeper " << std::endl;
        rc = eventWait(&vsync_event, UINT64_MAX);
        if (R_FAILED(rc))
            fatalSimple(rc);

        // now we can do our shit i guess

        // only player 1 can start the recording
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
        // if those two are down at the sametime we toggle recording
        if (((kDown & KEY_PLUS) && (kHeld & KEY_MINUS)) ||
            ((kDown & KEY_MINUS) && (kHeld & KEY_PLUS))) {

            record = !record;

            logger->info("Shortcut used\n");

            // currFrame = 0;
            if (record) {
                logger->info("Recording starts..\n");

                writer = new Helper::RecordWriter();
                // fs << "writer created" << std::endl;
                // get number of controllers here.
                // for now we will use P1_auto thing
                recorders.push_back(new InputRecorder(CONTROLLER_P1_AUTO));
                // fs << "pushed back the new InputRecorder in the recorders
                // array"
                //   << std::endl;
            }

            else {
                logger->info("Recording stops..\n");
                // deleting the pointers
                for (auto &i : recorders) {
                    delete i;
                }
                // clearing the array
                recorders.clear();
                if (writer != nullptr) {

                    delete writer;
                    writer = nullptr;
                }
            }
        }
        // if record
        if (record) {

            // getting all the infos from controllers
            // fs << "getting info " << std::endl;
            for (auto &i : recorders) {
                writer->AddInputs(i->Record());
            }
            // writing them to the destination file
            writer->WriteInfos();
        }
    }
    // Deinitialization and resources clean up code can go here.
    return 0;
}
