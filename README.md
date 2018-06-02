# Proteus
Client running on ESP8266, talking to TECT Backends

Proteus (c) 2018 by [Krogoth](https://twitter.com/le_krogoth) of [Ministry of Zombie Defense](http://www.mzd.org.uk/) and contributing authors

# Requirements
If you haven't done so before. Make sure to install the platformio toolset.

> pip install -U platformio

# Build
After cloning this repo. In root folder of Proteus, run this:

> platformio run

This builds the binary (.elf as well as .bin) into ~/.pioenvs/heltec_wifi_kit_8/ folder. 
Just place the new .bin file into the firmware folder of your TECT installation to 
have it installed OTA.

# Initial build
If you want to directly flash the firmware (via USB), run this:

> platformio run -t upload

If the upload crashes, check the platformio.ini for specific ports. Sometimes it also crashes the first time and 
then runs after that like a charm. Just try a second time when you are sure that 
the ports are correct.

If you want to build and upload the SPIFFS image as well, run this command:

> platformio run -t uploadfs 

You will have to flash the SPIFFS image at least once for a new installation of 
Proteus.

# Prerequisites
To build Proteus, you will need a working installation of platformio.

We use CLion and Visual Studio Code as code editors, your may want to use another editor.

If you use CLion, you might want to run this command at least once so that the CLion 
integration gets refreshed (which it sometimes seems to need):

> pio init --ide clion --board heltec_wifi_kit_8

# Architecture
proteus.cpp is the main file. This is where everything is set up and where the main 
loop can be found.

Proteus is using the concept of Modules or Modes to run. Think of it as a very abstract 
version of an application. Whenever you boot Proteus, it will check which Module to instantiate 
and run. Only one Module can be active at any time. If you switch Modules, you remove the 
old one from memory and initialise the next one.

The EventHandler manages your inputs from the three Buttons (Left, Right and Programm). 
Depending on the active Module, the events are handled differently. While Proteus manages 
some events, every Module can handle its own events (as long as it is active):

The ModeManager handles the switching of Modules. It makes sure that the memory is removed 
(at least to some extent, microcontrollers have a nifty kind of "garbage collector") and
that the new Module is initialised correctly and running.

The DisplayManager actually does not do the painting, but contains functionality which 
can be used by the Modules to paint to the screen. The painting itself is done by the 
Modules. The DisplayManager enforces a constant framerate, but this can be overriden 
by the Modules.

# Modules
This section will be filled later on.called

# Shoulders to stand on
The Proteus software was inspired somewhat by code made for the Arduboy.


# Licence
Proteus is published under the AGPLv3. See LICENCE file for details.