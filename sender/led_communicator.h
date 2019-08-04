#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include "device_manager.h"

using namespace std;
using namespace std::chrono;

class LED_Communicator
{
public:

    // functions
    static LED_Communicator* Instance();
    void display_string(string str);
    void clear_led_communicator();
    // variables

private:
    LED_Communicator();
    ~LED_Communicator();
    void turn_on_led_list();
    void turn_off_led_list();
 
    static LED_Communicator* instance;
    vector<short> gpio_list;
    Device_Manager* dev;
};
