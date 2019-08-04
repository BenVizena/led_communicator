g++ -Wall -Wno-psabi image_capturer.cpp main.cpp -o test3 `pkg-config --cflags --libs opencv` -pthread
