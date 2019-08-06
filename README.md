# led_communicator

This LED communicator has two parts: a sending part, and a receiving part. Both parts have been implemented using Raspberry Pis;
one with an infrared pi-camera, and the other with an array of LEDs with a 3D printed housing.

#### The receiver, which has to interpret the flashing LEDs, has most of the interesting software.
The receiver is meant to capture 
three imsages for every one character displayed by the sender and captures images at ~33 FPS. The captured images are stored in an 
internal structure. When the capturing thread has stored at least three images, the interpreting thread pulls a set of three images and 
determines what character was displayed by each of them.  The three images' characters count as votes to determine what character 
was being sent by the sender during the time that they were captured. If all three images vote differently, we pick the character from 
the middle (chronologically) image. To stay in sync during longer message transmissions, the receiver actively adjusts its framerate 
to try to keep all three images in each set in agreement as to which character was being transmitted by the sender.

#### The sender has most of the interesting hardware.
The sender accepts keyboard input from a terminal interface. When the enter key is pressed, the string is sent (flashed). 
Valid characters are A-Z, a-z, space, and period. The sender consists of 9 LED lights; 3 for orientation, and 6 for data transmission. 
The data LEDs are infrared LEDs, the orientation LEDs are blue. The housing for the LEDs is 3D printed.

[Here is an album that shows the encoding scheme, the actual hardware, and which pins the LEDs were connected to.](https://imgur.com/a/RU6Z7A7)

[And here is a video of it working.](https://www.youtube.com/watch?v=0uT-8nUPQME&feature=youtu.be)
The terminal on the right is the sender, and the terminal on the left is the receiver.
