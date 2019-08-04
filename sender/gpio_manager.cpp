#include "gpio_manager.h"


// GPIO setup macros. Always us INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock


GPIO_Manager* GPIO_Manager::instance = 0;


GPIO_Manager::GPIO_Manager()
{
    GPIO_Manager::instance = 0;
    
}

GPIO_Manager::~GPIO_Manager()
{
    vector<short> end_times{};
    for (int i = 0; i < 21; i++)
    {
        end_times = {(short)i};
        GPIO_Manager::turn_off_gpio_pins(end_times);
    }

//    delete instance;
}

GPIO_Manager* GPIO_Manager::Instance()
{
  
    if (instance == 0)
    {
        instance = new GPIO_Manager();
        instance->setup_io();
    }

    return instance;
}

bool GPIO_Manager::commission_gpio_pin(short pin)
{
    if (pin > 0 && gpio_commissioned_array[pin - 1] == false)
    {
        INP_GPIO(pin);
        OUT_GPIO(pin);
        gpio_commissioned_array[pin - 1] = true;
        return true;
    }
    else if (gpio_commissioned_array[pin - 1] == true)
    {
        return true;
    }
    else
    {
        cout << "invalid pin: " << pin << endl;
    }
    
    return false;
}

vector<bool> GPIO_Manager::turn_on_gpio_pins(vector<short> &pins)
{
    vector<bool> results;

    for (uint i = 0; i < pins.size(); i++)
    {
        if (pins[i] > 0 && gpio_taken_array[pins[i] - 1] == false)
        {
            if (commission_gpio_pin(pins[i])) // true if valid pin.  commissions pin if it wasn't already commissioned.
            {
                GPIO_SET = 1<<pins[i];
                gpio_taken_array[pins[i] - 1] = true;
                results.push_back(true);
            }
        }
        else if (gpio_taken_array[pins[i] - 1] == true)
        {
            cout << "tried to turn on pin that is already on: " << pins[i] << endl;
            results.push_back(false);
        }
        else
        {
            cout << "invalid pin: " << pins[i] << endl;
            results.push_back(false);
        }
    }
    
    return results;
}

vector<bool> GPIO_Manager::turn_off_gpio_pins(vector<short> &pins)
{
    vector<bool> results;

    for (uint i = 0; i < pins.size(); i++)
    {
        GPIO_CLR = 1<<pins[i];
        gpio_taken_array[pins[i] - 1] = false;
        results.push_back(true);
    }

    return results;
}

void GPIO_Manager::setup_io()
{
    /* open /dev/mem */
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
    {
        printf("can't open /dev/mem \n");
        exit(-1);
    }

    /* mmap GPIO */
    gpio_map = mmap(
        NULL,             //Any adddress in our space will do
        BLOCK_SIZE,       //Map length
        PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
        MAP_SHARED,       //Shared with other processes
        mem_fd,           //File to map
        GPIO_BASE         //Offset to GPIO peripheral
    );

    close(mem_fd); // No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED)
    {
        printf("mmap error %d\n", (int)gpio_map); // errno also set!
        exit(-1);
    }

    gpio = (volatile unsigned *)gpio_map;

}
