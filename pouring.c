#include <stdio.h>
#include <inttypes.h>
#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "linux/gpio.h"
#include "sys/ioctl.h"
#include "gpio.h"
#include "pouring.h"

int fdp, result;
struct gpiohandle_request req;

void error(){ //Error checking function
	printf("Error number: %s\n", strerror(errno));//Printing the error number and information from errno
	close(fdp); //closing the file
	exit(1); //exiting the linux gpio
}

int gpio_init(){
	fdp = gpio_fd;

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

int pour(pour_state_t on_off) {
	static pour_state_t previous_state;

	struct gpiohandle_data data;
	data.values[0] = NOT_POURING; //value to output (0 or 1)
	result = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data); //IO control asking to request to change lines at the data value
	if(result != 0){
		error();
	}

	if(previous_state != on_off){
		data.values[0] = POURING; //data values to 1 to turn on LED
		result = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		if(result != 0){	//checking if rv is equal to zero again
			error();
		}

		usleep(250000);

		data.values[0] = NOT_POURING;
		result = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		if(result != 0){	//checking if rv is equal to zero again
			error();
		}	
	}

	previous_state = on_off;	
	return 0;

}
