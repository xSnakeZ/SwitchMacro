#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define MAX_PULSES 10
#define DEFAULT_PULSE_DURATION_US 40000
#define LONG_PULSE_DURATION_US 3000000
//#define LONG_PULSE_DURATION_US 30000


#define MACRO_TIMING_START 31355
#define MACRO_TIMING_SAVEFILE 26637
#define MACRO_TIMING_OVERWORLD 7534

#define MACRO_PRESS_1 MACRO_TIMING_START
#define MACRO_PRESS_2 MACRO_PRESS_1 + MACRO_TIMING_SAVEFILE
#define MACRO_PRESS_3 MACRO_PRESS_2 + MACRO_TIMING_OVERWORLD

typedef struct {
    uint gpio;
    uint64_t end_time;
    uint32_t duration_us;
    bool active;
} pulse_t;

pulse_t pulses[MAX_PULSES];

typedef struct {
    char key;
    uint32_t delay;
} macro_step_t;

macro_step_t macro[] = {
    {'f', 0},
    {'f', MACRO_PRESS_1},
    {'f', MACRO_PRESS_2},
    {'f', MACRO_PRESS_3}, 
};

macro_step_t macro1[] = {
    {'f', 0},
    {'f', MACRO_PRESS_1},
};

int macro_len = sizeof(macro) / sizeof(macro_step_t);
uint64_t macro_start = 0;
int macro_index = 0;
bool macro_running = false;

const uint GPIO_MAP[] = {
    [0] = 255,
    ['a'] = 5,
    ['d'] = 4,
    ['f'] = 7,
    ['g'] = 6,
    ['h'] = 11,
    ['j'] = 10,
    ['s'] = 3,
    ['w'] = 2,
};

inline uint get_gpio(char key) {
    return (key < 128) ? GPIO_MAP[(unsigned char)key] : 255;
}

void start_pulse(uint gpio, uint64_t now, uint32_t duration_us) {
    for (int i = 0; i < MAX_PULSES; i++) {
        if (!pulses[i].active) {
            pulses[i].gpio = gpio;
            pulses[i].duration_us = duration_us;
            pulses[i].end_time = now + duration_us;
            pulses[i].active = true;
            gpio_set_dir(gpio, GPIO_OUT); 
            gpio_put(gpio, 0);     
            break;
        }
    }
}

void update_pulses(uint64_t now) {
    for (int i = 0; i < MAX_PULSES; i++) {
        if (pulses[i].active && now >= pulses[i].end_time) {
            gpio_put(pulses[i].gpio, 1); 
            gpio_set_dir(pulses[i].gpio, GPIO_IN);
            pulses[i].active = false;
        }
    }
}

void start_macro(uint64_t now) {
    macro_start = now;
    macro_index = 0;
    macro_running = true;
}

void update_macro(uint64_t now) {
    if (!macro_running || macro_index >= macro_len) {
        macro_running = false;
        return;
    }

    if (now - macro_start >= (uint64_t)macro[macro_index].delay * 1000) {
        uint gpio = get_gpio(macro[macro_index].key);
        if (gpio != 255) {
            uint32_t duration = (macro_index == 1) ? LONG_PULSE_DURATION_US : DEFAULT_PULSE_DURATION_US;
            start_pulse(gpio, now, duration);
        }
        macro_index++;
    }
}

int main() {
    stdio_init_all();

    uint pins[] = {2, 3, 4, 5, 6, 7, 10, 11};
    for (int i = 0; i < 8; i++) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
    }

    printf("Ready\n");

    while (true) {
        uint64_t now = time_us_64();

        int c = getchar_timeout_us(100);
        if (c != PICO_ERROR_TIMEOUT) {
            if (c == 'm') {
                start_macro(now);
            } else {
                uint gpio = get_gpio((char)c);
                if (gpio != 255) {
                    start_pulse(gpio, now, DEFAULT_PULSE_DURATION_US);
                }
            }
        }

        update_macro(now);
        update_pulses(now);

        sleep_us(10);
    }

    return 0;
}
