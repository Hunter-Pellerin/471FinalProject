#ifdef DISPLAY_H
#define DISPLAY_H

int init_display(void);
int display_oz(int fd, int button_value);
int display_pouring(int fd);

#endif