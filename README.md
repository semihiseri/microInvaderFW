# miniInvaderFW

## Getting started
Install the Espressif toolchain: https://docs.espressif.com/

Run menuconfig and ensure the bluetooth classic is enabled and BLE is disabled:

    idf.py menuconfig

Build and flash the project:

    idf.py build
    idf.py -p [port] flash
