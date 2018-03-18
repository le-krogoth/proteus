# Proteus
Client running on Microcontroller talking to TECT Backends

Proteus (c) 2018 by [Krogoth](https://twitter.com/le_krogoth) of [Ministry of Zombie Defense](http://www.mzd.org.uk/) and contributing authors

# Build
In root folder:

> platformio run

Builds binary (.elf as well as .bin) into ~/.pioenvs/heltec_wifi_kit_8/ folder. 
Just place the new .bin file into the firmware folder of your TECT installation to 
have it installed OTA.

# Initial build
If you want to flash the firmware for the first time, run this:

> platformio run --target upload

If the upload crashes, check the platformio.ini for specific ports.


# Prerequisites
To build Proteus, you will need a working installation of platformio.
We use CLion as code editor, your may want to use another editor.

# Licence
Proteus is published under the AGPLv3. See LICENCE file for details.