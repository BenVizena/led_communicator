#include "led_communicator.h" 

#define DURATION 90
#define o0 21
#define o1 20
#define d0 6
#define d1 12
#define d2 13
#define d3 19
#define d4 16
#define d5 26
#define o2 5

const short led_A[] = {o0, o1, o2, d0};
const short led_B[] = {o0, o1, o2, d1};
const short led_C[] = {o0, o1, o2, d0, d1};
const short led_D[] = {o0, o1, o2, d2};
const short led_E[] = {o0, o1, o2, d2, d0};
const short led_F[] = {o0, o1, o2, d2, d1};
const short led_G[] = {o0, o1, o2, d2, d1, d0};
const short led_H[] = {o0, o1, o2, d3};
const short led_I[] = {o0, o1, o2, d3, d0};
const short led_J[] = {o0, o1, o2, d3, d1};
const short led_K[] = {o0, o1, o2, d3, d1, d0};
const short led_L[] = {o0, o1, o2, d3, d2};
const short led_M[] = {o0, o1, o2, d3, d2, d0};
const short led_N[] = {o0, o1, o2, d3, d2, d1};
const short led_O[] = {o0, o1, o2, d3, d2, d1, d0};
const short led_P[] = {o0, o1, o2, d4};
const short led_Q[] = {o0, o1, o2, d4, d0};
const short led_R[] = {o0, o1, o2, d4, d1};
const short led_S[] = {o0, o1, o2, d4, d1, d0};
const short led_T[] = {o0, o1, o2, d4, d2};
const short led_U[] = {o0, o1, o2, d4, d2, d0};
const short led_V[] = {o0, o1, o2, d4, d2, d1};
const short led_W[] = {o0, o1, o2, d4, d2, d1, d0};
const short led_X[] = {o0, o1, o2, d4, d3};
const short led_Y[] = {o0, o1, o2, d4, d3, d0};
const short led_Z[] = {o0, o1, o2, d4, d3, d1};
const short led_SPACE[] = {o0, o1, o2, d4, d3, d1, d0};
const short led_PERIOD[] = {o0, o1, o2, d4, d3, d2};
const short led_NULL[] = {};


LED_Communicator* LED_Communicator::instance = 0;

LED_Communicator::LED_Communicator()
{
    instance = 0;
}

LED_Communicator::~LED_Communicator()
{
}

LED_Communicator* LED_Communicator::Instance()
{
    if (instance == 0)
    {
        instance = new LED_Communicator();
        instance->gpio_list = {o0, o1, d0, d1, d2, d3, d4, d5, o2}; 
        instance->dev = new Device_Manager(instance->gpio_list);
    }

    return instance;
}

void LED_Communicator::turn_on_led_list()
{
    instance->dev->turn_on_gpio_pins();
}

void LED_Communicator::turn_off_led_list()
{
    instance->dev->turn_off_gpio_pins();
}

void LED_Communicator::clear_led_communicator()
{
    short del[9] = {o0, o1, d0, d1, d2, d3, d4, d5, o2};
    instance->dev->set_pins_to_act_on(del, sizeof(del) / sizeof(del[0]));
    instance->~LED_Communicator();
}

void LED_Communicator::display_string(string str)
{
    for (uint x = 0; x < str.size(); x++)
    {
        char input = str[x];
        cout << "input: " << input << endl;
        switch(input)
        {
            case 'A':
            case 'a':
                    instance->dev->set_pins_to_act_on(led_A, sizeof(led_A) / sizeof(led_A[0]));
                    break;
            case 'B':
            case 'b':
                    instance->dev->set_pins_to_act_on(led_B, sizeof(led_B) / sizeof(led_B[0]));
                    break;
            case 'C':
            case 'c':
                    instance->dev->set_pins_to_act_on(led_C, sizeof(led_C) / sizeof(led_C[0]));
                    break;
            case 'D':
            case 'd':
                    instance->dev->set_pins_to_act_on(led_D, sizeof(led_D) / sizeof(led_D[0]));
                    break;
            case 'E':
            case 'e':
                    instance->dev->set_pins_to_act_on(led_E, sizeof(led_E) / sizeof(led_E[0]));
                    break;
            case 'F':
            case 'f':
                    instance->dev->set_pins_to_act_on(led_F, sizeof(led_F) / sizeof(led_F[0]));
                    break;
            case 'G':
            case 'g':
                    instance->dev->set_pins_to_act_on(led_G, sizeof(led_G) / sizeof(led_G[0]));
                    break;
            case 'H':
            case 'h':
                    instance->dev->set_pins_to_act_on(led_H, sizeof(led_H) / sizeof(led_H[0]));
                    break;
            case 'I':
            case 'i':
                    instance->dev->set_pins_to_act_on(led_I, sizeof(led_I) / sizeof(led_I[0]));
                    break;
            case 'J':
            case 'j':
                    instance->dev->set_pins_to_act_on(led_J, sizeof(led_J) / sizeof(led_J[0]));
                    break;
            case 'K':
            case 'k':
                    instance->dev->set_pins_to_act_on(led_K, sizeof(led_K) / sizeof(led_K[0]));
                    break;
            case 'L':
            case 'l':
                    instance->dev->set_pins_to_act_on(led_L, sizeof(led_L) / sizeof(led_L[0]));
                    break;
            case 'M':
            case 'm':
                    instance->dev->set_pins_to_act_on(led_M, sizeof(led_M) / sizeof(led_M[0]));
                    break;
            case 'N':
            case 'n':
                    instance->dev->set_pins_to_act_on(led_N, sizeof(led_N) / sizeof(led_N[0]));
                    break;
            case 'O':
            case 'o':
                    instance->dev->set_pins_to_act_on(led_O, sizeof(led_O) / sizeof(led_O[0]));
                    break;
            case 'P':
            case 'p':
                    instance->dev->set_pins_to_act_on(led_P, sizeof(led_P) / sizeof(led_P[0]));
                    break;
            case 'Q':
            case 'q':
                    instance->dev->set_pins_to_act_on(led_Q, sizeof(led_Q) / sizeof(led_Q[0]));
                    break;
            case 'R':
            case 'r':
                    instance->dev->set_pins_to_act_on(led_R, sizeof(led_R) / sizeof(led_R[0]));
                    break;
            case 'S':
            case 's':
                    instance->dev->set_pins_to_act_on(led_S, sizeof(led_S) / sizeof(led_S[0]));
                    break;
            case 'T':
            case 't':
                    instance->dev->set_pins_to_act_on(led_T, sizeof(led_T) / sizeof(led_T[0]));
                    break;
            case 'U':
            case 'u':
                    instance->dev->set_pins_to_act_on(led_U, sizeof(led_U) / sizeof(led_U[0]));
                    break;
            case 'V':
            case 'v':
                    instance->dev->set_pins_to_act_on(led_V, sizeof(led_V) / sizeof(led_V[0]));
                    break;
            case 'W':
            case 'w':
                    instance->dev->set_pins_to_act_on(led_W, sizeof(led_W) / sizeof(led_W[0]));
                    break;
            case 'X':
            case 'x':
                    instance->dev->set_pins_to_act_on(led_X, sizeof(led_X) / sizeof(led_X[0]));
                    break;
            case 'Y':
            case 'y':
                    instance->dev->set_pins_to_act_on(led_Y, sizeof(led_Y) / sizeof(led_Y[0]));
                    break;
            case 'Z':
            case 'z':
                    instance->dev->set_pins_to_act_on(led_Z, sizeof(led_Z) / sizeof(led_Z[0]));
                    break; 
            case ' ':
                    instance->dev->set_pins_to_act_on(led_SPACE, sizeof(led_SPACE) / sizeof(led_SPACE[0]));
                    break;
            case '.':
                    instance->dev->set_pins_to_act_on(led_PERIOD, sizeof(led_PERIOD) / sizeof(led_PERIOD[0]));
                    break;
            default:
                    instance->dev->set_pins_to_act_on(led_NULL, 0);
                    cout << "INVALID CHARACTER! TURNING OFF LEDs" << endl;
        }
        instance->turn_on_led_list();
        this_thread::sleep_until(system_clock::now() + milliseconds(DURATION - 5));
        instance->turn_off_led_list();
        this_thread::sleep_until(system_clock::now() + milliseconds(5));
    } 
}

