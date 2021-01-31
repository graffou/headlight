# headlight

Bicycle headlight SW. Works with lumotec cyo, using 5V USB battery. * Shunt headlight rectifier, then cut 7413 driver circuit gate (pin 4). 
* Connect this pin to pin 9 of arduino mini. 
* Connect a switch between gnd and pin 8 of arduino. 
* Connect headlight power supply (post headlight switch) to arduino. 
* One short press on switch changes between still / flash mode. 
* Long presses change headlight intensity (still mode) or flash frequency (flash mode). 
* The Arduino mini should vertically fit inside on the right side of the headlight case (from the cyclist POV), close to the original mainboard.
