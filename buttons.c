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
#include "gpio.h"

#define DOWN_GPIO_PIN 26
#define UP_GPIO_PIN 13
#define POUR_GPIO_PIN 19
#define NUM_READS_DEBOUNCE 3
#define MAX_BUTTONS 3
#define MAX_OUNCES 16

#define DOWN_BUTTON 0
#define UP_BUTTON 1
#define POUR_BUTTON 2

button_state_t buttons[MAX_BUTTONS];
button_state_t buttons_prev[MAX_BUTTONS];

// File descriptor array for GPIO lines
int gpio_fdb[MAX_BUTTONS];
int fd;

uint8_t ounces_to_pour = 8;
uint8_t pouring = 0;

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
        //close(fd);
        handleError();
    }

    // Store the file descriptor
    gpio_fdb[index] = req.fd;

    // Close the chip file descriptor (we only need req.fd for later use)
    //close(fd);
}

void init_buttons()
{
	// get file descriptor, open file for reading/writing
	fd = gpio_fd;

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

void handle_button(int idx)
{
    uint8_t stableCount = 0;

    // Read the state multiple times before asserting the state was changed for debouncing
    for (int i = 0; i < NUM_READS_DEBOUNCE; i++)
    {
        buttons[idx] = (button_state_t)read_button(gpio_fdb[idx]);

        // Compare the state with the previous state
        if (buttons_prev[idx] != buttons[idx])
        {
            stableCount++;
        }
        else
        {
            // State remains unchanged, break early to debounce
            usleep(1000);
            break;
        }

        // Wait for 5ms between checks
        usleep(5000);
    }

    // If stable state detected, process it
    if (stableCount == NUM_READS_DEBOUNCE)
    {
        if (buttons[idx] == BUTTON_PRESSED && buttons_prev[idx] == BUTTON_RELEASED)
        {
            // Only execute actions on state change from released to pressed
            switch (idx)
            {
            case POUR_BUTTON:
                pouring = !pouring;
                break;

            case DOWN_BUTTON:
                if (ounces_to_pour > 0)
                {
                    ounces_to_pour--;
                }
                break;

            case UP_BUTTON:
                if (ounces_to_pour < MAX_OUNCES)
                {
                    ounces_to_pour++;
                }
                else
                {
                    ounces_to_pour = MAX_OUNCES;
                }
                break;
            }
        }
    }

    // Update the previous button state for the next read
    buttons_prev[idx] = buttons[idx];
}