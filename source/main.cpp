// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Include the main libnx system header, for Switch development
#include <switch.h>





// Adjust size as needed.
#define INNER_HEAP_SIZE 0x80000

// testing purposes
#include <map>
#include <list>
#include <string>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <cstring>
extern "C" 
{
	// Sysmodules should not use applet*.
	u32 __nx_applet_type = AppletType_None;
	size_t nx_inner_heap_size = INNER_HEAP_SIZE;
	char   nx_inner_heap[INNER_HEAP_SIZE];

	void __libnx_initheap(void)
	{
		void*  addr = nx_inner_heap;
		size_t size = nx_inner_heap_size;

		// Newlib
		extern char* fake_heap_start;
		extern char* fake_heap_end;

		fake_heap_start = (char*)addr;
		fake_heap_end   = (char*)addr + size;
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

		//Enable this if you want to use time.
		/*rc = timeInitialize();
		  if (R_FAILED(rc))
		  fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_Time));

		  __libnx_init_time();*/

		rc = fsInitialize();
		if (R_FAILED(rc))
			fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));


		//init vsync events?
		rc = viInitialize(ViServiceType_System);
		if(R_FAILED(rc))
			fatalSimple(rc);

		// need this to get pId
		rc = pmdmntInitialize();
		if(R_FAILED(rc))
			fatalSimple(rc);

		// need this to get applicationid
		rc = pminfoInitialize();
		if(R_FAILED(rc))
			fatalSimple(rc);

		//setting hos version because apparently it changes some functions
		rc = setsysInitialize();
		if (R_SUCCEEDED(rc)) {
			SetSysFirmwareVersion fw;
			rc = setsysGetFirmwareVersion(&fw);
			if (R_SUCCEEDED(rc))
				hosversionSet(MAKEHOSVERSION(fw.major, fw.minor, fw.micro));
			setsysExit();
		}
		rc = nsInitialize();
		if(R_FAILED(rc))
			fatalSimple(rc);
		

		fsdevMountSdmc();
	}

	void __attribute__((weak)) userAppExit(void);
	void __attribute__((weak)) __appExit(void)
	{
		// Cleanup default services.
		fsdevUnmountAll();
		fsExit();

		//timeExit();//Enable this if you want to use time.
		hidExit();// Enable this if you want to use HID.
		smExit();
		pmdmntExit();
		nsExit();
	}

}// end extern C
// Main program entrypoint
int main(int argc, char* argv[])
{
	// Initialization code can go here.

	// Your code / main loop goes here.
	// If you need threads, you can use threadCreate etc.
	bool record = false;
	int currFrame = 0;	

	// "reverse" map keys to their text counterpart
	/*
	   const std::map<int, std::string> bla = {
	   {1<<0,"KEY_A"},
	   {1<<1,"KEY_B"},
	   {1<<2,"KEY_X"},
	   {1<<3,"KEY_Y"},
	   {1<<4,"KEY_LSTICK"},
	   {1<<5,"KEY_RSTICK"},
	   {1<<6,"KEY_L"},
	   {1<<7,"KEY_R"},
	   {1<<8,"KEY_ZL"},
	   {1<<9,"KEY_ZR"},
	   {1<<10,"KEY_PLUS"},
	   {1<<11,"KEY_MINUS"},
	   {1<<12,"KEY_DLEFT"},
	   {1<<13,"KEY_DUP"},
	   {1<<14,"KEY_DRIGHT"},
	   {1<<15,"KEY_DDOWN"},
	   {1<<16,"KEY_LSTICK_LEFT"},
	   {1<<17,"KEY_LSTICK_UP"},
	   {1<<18,"KEY_LSTICK_RIGHT"},
	   {1<<19,"KEY_LSTICK_DOWN"},
	   {1<<20,"KEY_RSTICK_LEFT"},
	   {1<<21,"KEY_RSTICK_UP"},
	   {1<<22,"KEY_RSTICK_RIGHT"},
	   {1<<23,"KEY_RSTICK_DOWN"},
	   {1<<24,"KEY_SL_LEFT"},
	   {1<<25,"KEY_SR_RIGHT"},
	   {1<<26,"KEY_SL_RIGHT"},
	   {1<<27,"KEY_SR_RIGHT"},
	   };
	 */
	std::fstream fs;
	Event vsync_event;


	Result rc;
	ViDisplay disp;
	rc = viOpenDefaultDisplay(&disp);
	if(R_FAILED(rc))
		fatalSimple(rc);
	rc = viGetDisplayVsyncEvent(&disp, &vsync_event);
	if(R_FAILED(rc))
		fatalSimple(rc);


	while (true)
	{

		//wait on screen refresh i guess
		// uh whats a good timeout kev
		rc = eventWait(&vsync_event,UINT64_MAX );
		if (R_FAILED(rc))
			fatalSimple(rc);


		//now we can do our shit i guess


		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
		// if those two are down at the sametime we start recording
		if (((kDown & KEY_PLUS) && (kHeld & KEY_MINUS))
				|| ((kDown & KEY_MINUS) && (kHeld & KEY_PLUS)))
		{
			record = !record;
			currFrame = 0;	
			if (record)
			{
				fs.open("/input-recorder/test.txt",std::fstream::out);
				std::string filename;
				//getting title ID
				u64 pid;
				if (R_FAILED(pmdmntGetApplicationPid(&pid)))
				{
					fs << std::endl << "couldn't read pid feelsdankman";
				}
				else
				{
					// Getting pID to get
					u64 title;
					fs << std::endl << "process id:" << pid;
					// Getting title ID as ex
					Result wut = pminfoGetTitleId(&title,pid);
					if (R_FAILED(wut))
					{
						// if we cant get it, the directory should be unknown i guess
						//fatalSimple();
					}	
					else
 					{
					}



				}


			}

			else
			{
				//trying to  find out wtf we recorded:

				fs.close();
			}

		}

		if (record)
		{

			//https://switchbrew.github.io/libnx/structJoystickPosition.html
			JoystickPosition lPos;			
			JoystickPosition rPos;	
			//reading left & right joystick pos
			hidJoystickRead(&lPos,CONTROLLER_P1_AUTO,JOYSTICK_LEFT);
			hidJoystickRead(&rPos,CONTROLLER_P1_AUTO,JOYSTICK_RIGHT);

			// write them i guess
			//recording the joystick state

			//https://switchbrew.github.io/libnx/hid_8h.html	


			//writing current frame as well as the pressed buttons and the sticks state
			fs << kHeld << " " << lPos.dx << ";" << lPos.dy << " " << rPos.dx << ";" << rPos.dy  << std::endl;

			currFrame++;

		}


	}
	// Deinitialization and resources clean up code can go here.
	return 0;

}
