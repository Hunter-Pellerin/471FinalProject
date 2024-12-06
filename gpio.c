#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"

int gpio_fd = -1;

void open_gpio()
{
    gpio_fd = open("/dev/gpiochip0", O_RDWR); // Open for both read and write
    if (gpio_fd < 0)
    {
        perror("Failed to open /dev/gpiochip0");
        exit(EXIT_FAILURE);
    }
}

void cleanup_gpio()
{
    if (gpio_fd >= 0)
    {
        close(gpio_fd);
    }
}