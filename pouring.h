#ifdef POURING_H
#define POURING_H

// Pour states
typedef enum {
    NOT_POURING,
    POURING
} pour_state_t;

int pour(pour_state_t on_off);
int gpio_init();

#endif