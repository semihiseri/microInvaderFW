# miniInvaderFW

## Getting started
Prepare yourself for an adventure in robotics! And remember, the real treasure in programming is not the result but the bugs we make along the way.

### Assembly
Connect the right motor to connector J2 and the left motor to J3.

### Configure Wi-Fi or Bluetooth via serial
* Connect board to computer via USB
* Establish [serial connection](https://docs.espressif.com/projects/esp-idf/en/release-v4.1/get-started/establish-serial-connection.html)

You can connect for example using screen:

    screen [port] 115200

### Controlling the robot

Once you have set up the Wi-Fi connection, you should be able to control the robot by sending UDP messages to port 3000. The message should be a string consisting of two integers between 100 and -100 separated by a semicolon. You can try it out e.g. with the following command:

    echo -n '50;-50' | nc -u [robot ip] 3000

Or by modifying and running the [python example](examples/send-udp.py)

---

## Development

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