# ESP32 Maker PCB v3.0
## Documentation and example code for usage exploration of:
 - basic electronic functions
 - LCD, OLED and LED displays
 - servo, stepper and drive motor control
 - SPIFFS and SD card reader storage
 - various sensors, and
 - web server set ups to control Kit connected items from a browser

  &nbsp; &nbsp; <img src="images/ESP32_PCB03_annotated_900w.jpg" width="600" height="357">

  A robust permanent assembly, that connects to the GPIO pins of a 38 pin ESP32 module, can be created by soldering a set of components into a custom Printed Circuit Board (PCB v3.0). The module design, as shown above, includes a number of on-board devices that can be controlled by the ESP32 (e.g. buzzer, LEDs, etc.), but also allows a wide range of further components to be connected to the PCB enabling more ESP32 controlled systems to be explored.

<img src="images/ESP32_PCB03_front01_900w.jpg" width="217" height="180"> &nbsp; &nbsp; <img src="images/ESP32_PCB03_20211109_113014124_updated_900w.jpg" width="204" height="180"> &nbsp; &nbsp; <img src="images/ESP32_PCB03_20211109_113130107blurred_900w.jpg" width="209" height="180"> 

Full details about the project are published at <a href="https://onlinedevices.org.uk/ESP32+Maker+PCB" target="_blank" >this link</a> and the designs for a small number of associated custom 3D printed components can be downloaded from the Prusa web site <a href="https://www.prusaprinters.org/prints/87023-esp32-maker-pcb-stands" target="_blank" >here</a>.

## Custom PCB design

The custom PCB (now at v3.0) was designed using KiCAD and the layout has been defined to provide all the components needed to build a fully populated module.

Gerber files for the PCB design can be downloaded from the PCB_design_files folder.

## Example code

All the example code was developed using the Arduino IDE and various standard/readily available libraries. Of particular note is the use of the TFT_eSPI library for use with SPI displays, where a Customs_Setup folder is provided which should be added to the display library folder along with a User_Setup_Select.h file, also added to the library folder, which provides the custom path references.
