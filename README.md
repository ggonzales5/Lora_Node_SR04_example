# Lora_Node_SR04_example
SR04 Ultrasonic Sensor comtrol node on Lora WiFI 32

For compilation, flashing for the ESP32 follow
isntallation instructions for your operating
system at: 

https://github.com/espressif/arduino-esp32#installation-instructionshttps://github.com/olikraus/u8g2https://robotzero.one/heltec-wifi-kit-32/

Then unzip the file xtensa-toolchain-rpi.zip included in this repo in
the following directory:

~/Arduino/hardware/espressif/esp32/tools

This will create the needed toolchain for teh raspberry pi 3 in
the created xtensa-esp32-elf/ directory.

Note that the following library:

https://github.com/olikraus/u8g2

is required to implement the on screen display
elements of this program. This library can also
be installed via the "manage libraries" tab in
the arduino ide.
