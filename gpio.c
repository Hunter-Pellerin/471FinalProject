#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"

// Initialize gpio fd for gpio i/o
int gpio_fd = -1;

// Open gpio
void open_gpio()
{
    gpio_fd = open("/dev/gpiochip0", O_RDWR); // Open for both read and write
    if (gpio_fd < 0)
    {
        // If an error occurred, need to exit
        perror("Failed to open /dev/gpiochip0");
        exit(EXIT_FAILURE);
    }
}

// Cleanup: close gpio file if it is open
void cleanup_gpio()
{
    if (gpio_fd >= 0)
    {
        close(gpio_fd);
    }
}