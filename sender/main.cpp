#include "led_communicator.h"
#include <signal.h>
#include <unistd.h>
#include <thread>


bool running = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        cout << "received SIGINT" << endl;
        running = false;
    }
}

int main()
{
    signal(SIGINT, sig_handler);

    while(running)
    {
        char input[200];
        cout << "Enter string to flash: ";
        cin.getline(input,sizeof(input));
        //cin >> input;
        std::cout << "intput is: " << input << std::endl;
        LED_Communicator::Instance()->display_string(input);
//        input = "";

    }

    return 0;
}
