#include <stdio.h>
#include <inttypes.h>
#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "linux/gpio.h"
#include "sys/ioctl.h"
#include <sys/time.h>
#include "gpio.h"
#include "pouring.h"
#include "buttons.h"

const double OZ_TO_SEC = 37.f/32.f;

int fdp, result;
struct gpiohandle_request req;

//Error checking function
void error(){
	printf("Error number: %s\n", strerror(errno));//Printing the error number and information from errno
	close(fdp); //closing the file
	exit(1); //exiting the linux gpio
}

// Initialize the gpio output for the relay
int gpio_init(){
	// Use gpio file from gpio.h
	fdp = gpio_fd;

	// Initialize gpio outputs for pump relay
	memset(&req,0,sizeof(struct gpiohandle_request));
	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	req.lines = 1;
	req.lineoffsets[0] = 17; 				//Offset to specified GPIO pin
	req.default_values[0] = 0;
	strcpy(req.consumer_label, "ECE471");
	result = ioctl(fdp, GPIO_GET_LINEHANDLE_IOCTL, &req);

	if(result != 0){	//rv will equal to 0 if the operation is successful
		error();//if not run error check function
	}
	
	return 0;
}

// Toggles the relay on for 250ms then turn off on an input parameter state change
int pour(pour_state_t on_off) {
    static pour_state_t previous_state = NOT_POURING;
    static double secs_to_pour = 0;
    static struct timeval last_time;

    struct timeval current_time;
    double time_passed;
    struct gpiohandle_data data;
    int result;

    data.values[0] = NOT_POURING;

	// If previous pouring state does not match desired state, need to toggle pouring relay
    if (previous_state != on_off) {
        // If pouring, calculate seconds to pour based on ounce selection
        if (on_off == POURING) {
            secs_to_pour = ounces_to_pour * OZ_TO_SEC;
            gettimeofday(&last_time, NULL); // Initialize last_time
        }

        data.values[0] = POURING;
        result = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
        if (result != 0){ // check result to see if gpio read succeeded
			error();
		}

        usleep(250000); // 250 ms delay to allow pump to turn on, mimics a button press

        data.values[0] = NOT_POURING;
        result = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
        if (result != 0){ // check result to see if gpio read succeeded
			error();
		}

        previous_state = on_off;
    }

	//
    if (on_off == POURING) {
        gettimeofday(&current_time, NULL);
        time_passed = (current_time.tv_sec - last_time.tv_sec) +
                      (current_time.tv_usec - last_time.tv_usec) / 1e6;
        last_time = current_time; // Update last_time to current_time

        secs_to_pour -= time_passed;
        if (secs_to_pour <= 0) {
            secs_to_pour = 0;
            pouring = NOT_POURING; // Stop pouring
        }


    }

    return 0;
}

