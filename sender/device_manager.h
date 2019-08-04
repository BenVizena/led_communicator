#pragma once

#include "gpio_manager.h"

class Device_Manager
{
    public:
        Device_Manager(vector<short> pins);
        ~Device_Manager();
        vector<bool> turn_on_gpio_pins();
        vector<bool> turn_off_gpio_pins();
        vector<bool> set_pins_to_act_on(const short pins[], int size);

        std::vector<short> pins_to_act_on{};
    private:
        static vector<bool> can_take_pins(vector<short> pins);

        static array<bool, 21> pins_list;
        array<bool, 21> my_pins{};
};
