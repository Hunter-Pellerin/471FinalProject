#include <stdio.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "linux/gpio.h"
#include "sys/ioctl.h"

int fd, result;

void error(){ //Error checking function
	printf("Error number: %s\n", strerror(errno));//Printing the error number and information from errno
	close(fd); //closing the file
	exit(1); //exiting the linux gpio
}

int gpio_init(fd){

	fd = open("/dev/gpiochip0", O_RDWR);

	struct gpiohandle_request req;
	memset(&req,0,sizeof(struct gpiohandle_request));
	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	req.lines = 1;
	req.lineoffsets[0] = 17; 				//Offset to specified GPIO pin
	req.default_values[0] = 0;
	strcpy(req.consumer_label, "ECE471");
	result = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);

	if(result != 0){	//rv will equal to 0 if the operation is successful
		error();//if not run error check function
	}
	
	return 0;
}

int pour(int on_off, fd){

	static uint8_t  previous_state;

	struct gpiohandle_data data;
	data.values[0] = 0; //value to output (0 or 1)
	result = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data); //IO control asking to request to change lines at the data value
	if(rv != 0){
		error();
	}

	if(previous_state != on_off){
		data.values[0] = on_off; //data values to 1 to turn on LED
		result = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		if(result != 0){	//checking if rv is equal to zero again
			error();
		}

		usleep(250000);

		data.values[0] = 0;
		result = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		if(result != 0){	//checking if rv is equal to zero again
			error();
		}	
	}

	previous_state = on_off;	
	return 0;

}