#include <stdio.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/gpio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>

void handleError()
{
	printf("Error # %s: opening file for GPIO read/write operations. \n", strerror(errno));
	exit(1);
}

int8_t read_button(int fd)
{
	// Declare data struct and init to zeros before getting gpio data
	struct gpiohandle_data data;

	memset(&data, 0, sizeof(data));
	int8_t rv = ioctl(fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);

	// Check the return value of ioctl
	if (0 != rv)
	{
		handleError();
		return 1;
	}

	// The read value is in data.values[0]
	return data.values[0];
}
