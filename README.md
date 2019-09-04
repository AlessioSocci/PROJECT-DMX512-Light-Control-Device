# Light-Control-Device-

Device Project Based on NXP KL25Z ARM Cortex M0+, 50 MHz microcontroller;
Light control with DMX 512 serial protocol.

Device is formed by:

n.8 encoder "KY040"
n.1 joystick 
N.1 OLED screen 0.96" dual-color with SSD1306 driver
N.1 transceiver UART-RS485 physical-layer
n.1 NXP FRDM KL25Z board
n.1 pannel connector xlr 3 pole
n.1 pannel connector USB 

When power on, OLED display a intro-screen and a backgroung pattern for bar-graph, with number of initial default eight dmx channel:
ch1 to ch8.

With stick pan move, eigth controlled dmx channel switch left-or right by one position:
one move of stick to the right --> dmx channel controlled are ch2 to ch 9; 
now, one move to the left --> dmx channel controlled are ch1 to ch8.

With stick tilt move, eigth controlled dmx channel switch left-or right by eight position:
one move of stick in up position --> dmx channel controlled are ch9 to ch 16; 
now, one move in down position --> dmx channel controlled are ch1 to ch8.

With stick push button, device enter in fine-controll mode, to allow fine change of every dmx channle value.


With anti-clockwise rotation of an encoder, correspond dmx channel increment;
with clockwise rotation, decrement.

With encoder push button, correspond dmx channel set at 100%, repushing button the correspond dmx channel return to previous value.

Display show eigth bar-graph correspond to value of the eight channel selected bu stick.

