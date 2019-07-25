# arduino-wearables

A collection of code powering wearable Arduino projects.

## Installation

0. Clone this repository somewhere on your local machine

1. Initialize the Git submodules in the `Libraries` directory:
```sh
cd path/to/arduino-wearables
git submodule update --init --recursive
```

2. Download (or update) the Arduino IDE: https://www.arduino.cc/en/main/software

3. Download and install the CP2104 USB driver: https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide

4. Under the **Arduino** > Preferences… menu:
  - Change the "Sketchbook location" to the `arduino-wearables` directory
  - Add the following URLs to the "Additional Boards Manager URLs" input box:
    ```
    https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
    http://arduino.esp8266.com/stable/package_esp8266com_index.json
    ```

5. Under **File** > Sketchbook, choose `led_bandolier`

6. Under the **Tools** > Board:… menu
  - Open the Boards Manager and make sure the `esp8266` board is installed
  - Select the "Adafruit Feather HUZZAH ESP8266" board

7. Check the following **Tools** menu items:
  - Board: "Adafruit Feather Huzzah ESP8266"
  - CPU Frequency "80 MHz"
  - Upload Speed: "115200"

6. Click the checkmark button in the upper left corner of the IDE to compile the `led_bandolier` sketch

7. Plug your bandolier in via USB and click the arrow button to upload the compiled code

8. Rainbows!
