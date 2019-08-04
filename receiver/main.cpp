#include "image_capturer.h"
#include <signal.h>

int running = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        running = false;
    }
}


int main()
{
    signal(SIGINT, sig_handler);

    std::thread process_thread(&Image_Capturer::start_processing_images, 90);
    std::thread capture_thread(&Image_Capturer::start_capturing_images, 90);

    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Image_Capturer::stop_capturing_images();
    capture_thread.join();
    process_thread.join();

    return 0;
}
