#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <array>

#define BCM2708_PERI_BASE   0x3F000000
#define GPIO_BASE           (BCM2708_PERI_BASE + 0x200000) // GPIO controller

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

using namespace std;

class GPIO_Manager
{
    public:
        static GPIO_Manager* Instance();
        vector<bool> turn_on_gpio_pins(vector<short> &pins);
        vector<bool> turn_off_gpio_pins(vector<short> &pins);

    private:
        GPIO_Manager();
        ~GPIO_Manager();
        void setup_io();
        bool commission_gpio_pin(short pin); 
        
        static GPIO_Manager* instance;
        int mem_fd;
        void *gpio_map;
        volatile unsigned *gpio; // I/O access
        bool gpio_taken_array[21] = {0}; // used to tell which gpio pins have been taken by a device
        bool gpio_commissioned_array[21] = {0}; // used to tell which gpio pins have had INP_GPIO called on them.
};
