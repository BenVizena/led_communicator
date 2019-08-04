#include "device_manager.h"

array<bool, 21> Device_Manager::pins_list;

Device_Manager::Device_Manager(vector<short> pins)
{
    pins_to_act_on.resize(9);
    vector<bool> i_can_haz_pins = Device_Manager::can_take_pins(pins);
    
    short bad_pin = 0;
    short had_bad_pin = false;

    for (uint x = 0; x < i_can_haz_pins.size(); x++)
    {
        if (i_can_haz_pins[x] == false)
        {
            had_bad_pin = true;
            bad_pin = pins[x];
            break;
        }
    }

    if (!had_bad_pin)
    {
        for (uint x = 0; x < pins.size(); x++)
        {

            my_pins[pins[x] - 1] = true;

            pins_list[pins[x] - 1] = true;
        }
    }
    else
    { 
        cout << "PIN " << bad_pin << " IS ALREADY TAKEN AND CANNOT BE ASSIGNED!" << endl;
    }

    cout << "SUCCESSFULLY SET UP DEVICE_MANAGER!" << endl;
}

Device_Manager::~Device_Manager()
{
}

vector<bool> Device_Manager::can_take_pins(vector<short> pins)
{
    vector<bool> results;

    for (uint i = 0; i < pins.size(); i++)
    {

        if (pins_list[pins[i] - 1] == false) // if pins_list[pin_num - 1] is not assigned, return true
        {
            results.push_back(true);
        }
        else
        {
            results.push_back(false);
        }
    }

    return results;
}

vector<bool> Device_Manager::turn_on_gpio_pins()
{
    return GPIO_Manager::Instance()->turn_on_gpio_pins(pins_to_act_on);
}

vector<bool> Device_Manager::turn_off_gpio_pins()
{
    return GPIO_Manager::Instance()->turn_off_gpio_pins(pins_to_act_on);
}

vector<bool> Device_Manager::set_pins_to_act_on(const short pins[], int size)
{
    pins_to_act_on.clear();

    bool can_turn_on_all_pins = true;
    vector<bool> results;

    for (int i = 0; i < size; i++)
    {
        if (my_pins[pins[i] - 1] == true)
        {
            results.push_back(true);
        }
        else
        {
            can_turn_on_all_pins = false;
            results.push_back(false);
        }
    }

    if (can_turn_on_all_pins)
    {
        for (int i = 0; i < size; i++)
        {
            short p = pins[i];
            pins_to_act_on.push_back(p);
        }
    }

    return results;
}

