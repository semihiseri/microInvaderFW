# miniInvaderFW

## Getting started
Install the Espressif toolchain: https://docs.espressif.com/projects/esp-idf/en/release-v4.1/

Run menuconfig and ensure that:
* bluetooth classic is enabled and BLE is disabled
* the custom partition table partitions.csv is selected
* at least 4MB flash size is selected from `Serial flasher config > Flash size`
```
    idf.py menuconfig
```

Build and flash the project:

    idf.py build
    idf.py -p [port] flash

Monitor:

    idf.py -p [port] monitor

To exit the monitor press `ctrl + ]` (`ctrl + 5` On a Finnish keyboard)