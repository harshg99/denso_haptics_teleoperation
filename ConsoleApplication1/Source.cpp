#include "stdafx.h"
#include <stdio.h>
#include "dhdc.h"



/*int
main(int  argc,
	char **argv)
{
	int done = 0;

	// get device count
	if (dhdGetDeviceCount() <= 0) {
		printf("error: no device found (%s)\n", dhdErrorGetLastStr());
		return 0;
	}

	// open the first available device
	if (dhdOpen() < 0) {
		printf("error: %s\n", dhdErrorGetLastStr());
		return 0;
	}
	if (dhdEmulateButton(1, dhdGetDeviceID()) < 0){
		printf(" lol error: %s\n", dhdErrorGetLastStr());
		return 0;
	}
	
	// haptic loop
	while (!done) {

		// apply a null force to put the device in gravity compensation
		if (dhdSetForce(0.0, 0.0, 0.0) < 0) {
			printf("error: %s\n", dhdErrorGetLastStr());
			done = 1;
		}

		// detect button click to quit the haptic loop
		if (dhdGetButton(0)) {
			printf("exiting...\n");
			done = 1;
		}
	}

	// close the connection to the device
	if (dhdClose() < 0) {
		printf("error: %s\n", dhdErrorGetLastStr());
	}

	return 0;
}*/