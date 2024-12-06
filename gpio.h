#ifndef GPIO_H
#define GPIO_H

extern int gpio_fd; // Global file descriptor
void open_gpio();
void cleanup_gpio();

#endif