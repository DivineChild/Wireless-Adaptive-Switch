# Wireless Adaptive Switch

This project was created for being able to control adaptive technology wireless via an ESP32. This project uses ESP-Now as its backend.

## Setup

(Possibly no longer needed in Arduino IDE)
Compiling this project by opening in Arduino IDE and hitting compile won't work, the library in `lib` will have to be added to your Arduino library under the folder `Wireless-Adaptive-Switch`, I recommend doing this via a symlink.

MAC OS Example:

    ln -s /Users/mad/Documents/Arduino/Wireless-Adaptive-Switch/lib/ /Users/mad/Documents/Arduino/libraries/Wireless-Adaptive-Switch

This means you can update the files in the `lib` folder without having to change code that the `sender` and `receiver` use.


(Appears to be in the board library by default now)
Install the espressif board library for compiling code as well as ESP-Now.
Place the below in your board manager in settings for the espressif boards.
    
    https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json

The board is an `ESP32 Dev Module` in Arduino IDE.

Additional header files must be added to `lib/Wireless-Adaptive-Switch.hpp`

## Next Steps

* Find a stable way to power the project.
* Add a pairing button
* Confirm clearing settings works
* Switch that will change the button to a toggle
* Enabled encryption, checkout `Pair.cpp`
