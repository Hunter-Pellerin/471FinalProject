#include "buttons.h"

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/gpio.h>
#include <sys/time.h>

#include "debounce.h"

#define DOWN_GPIO_PIN 17
#define UP_GPIO_PIN 18
#define POUR_GPIO_PIN 19
#define NUM_READS_DEBOUNCE 3
#define MAX_BUTTONS 3

button_state_t buttons[MAX_BUTTONS];
button_state_t buttons_prev[MAX_BUTTONS];

// File descriptor array for GPIO lines
int gpio_fd[MAX_BUTTONS];

int fd;

// Initialize a GPIO pin as input
void initGpioInput(int8_t pin, int index)
{
    // setup gpio request handler struct for button
    struct gpiohandle_request req;
    int rv;

    // Clear the request structure
    memset(&req, 0, sizeof(struct gpiohandle_request));
    req.flags = GPIOHANDLE_REQUEST_INPUT;
    req.lines = 1;
    req.lineoffsets[0] = pin;
    strcpy(req.consumer_label, "ECE471");

    // Request line handle
    rv = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
    if (rv != 0)
    {
        close(fd);
        handleError("Failed to get line handle");
    }

    // Store the file descriptor
    gpio_fd[index] = req.fd;

    // Close the chip file descriptor (we only need req.fd for later use)
    close(fd);
}

void init_buttons()
{
	// get file descriptor, open file for reading/writing
	fd = open("/dev/gpiochip0", O_RDWR);

	// if could not open gpio file for read/write
	if (-1 == fd)
	{
		handleError();
	}

	// initialize button inputs
	initGpioInput(DOWN_GPIO_PIN, DOWN_BUTTON);
    initGpioInput(UP_GPIO_PIN, UP_BUTTON);
    initGpioInput(POUR_GPIO_PIN, POUR_BUTTON);

    buttons[DOWN_BUTTON] = BUTTON_RELEASED;
    buttons[UP_BUTTON] = BUTTON_RELEASED;
    buttons[POUR_BUTTON] = BUTTON_RELEASED;
    buttons_prev[DOWN_BUTTON] = buttons[DOWN_BUTTON];
    buttons_prev[UP_BUTTON] = buttons[UP_BUTTON];
    buttons_prev[POUR_BUTTON] = buttons[POUR_BUTTON];
}

void get_button(int idx)
{
    uint8_t changeCount = 0;

    // read the state 3 times before claiming the state was changed for debouncing
    for (int i=0; i < NUM_READS_DEBOUNCE; i++)
    {
        buttons[idx] = (button_state_t)read_button(gpio_fd[0]);

        if (buttons_prev[idx] != buttons[idx])
        {
            changeCount++;
        }
        else
        {
            // wait for 1ms before trying again
            usleep(1000);
            break;
        }

        // wait for 5ms each cycle
        usleep(5000);
    }
    
    if (changeCount == NUM_READS_DEBOUNCE)
    {
        buttons_prev[idx] = buttons[idx];
    }

    return buttons_prev[idx];
}