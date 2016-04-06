# Sparki C Library

The goal of this repo is to adapt the Sparki library from ArcBotics/Sparki
into a Pure C library, insoasfar as is possible/practical.  
The benefit of this is it means it's much easier to write C code to control
the Sparki, without having to wrap C++ functions with a C interface.

The translation isn't so insane, since the Sparki source code doesn't make
use of many C++ features. (i.e. not much inheritance/polymorphism; only ever
one instance of a class; etc.). -- The main advantage to C++ code seems to be
the `print` functions inherited from Arduino's C++ `Print.h`; method/function
overloading is also used.

At this stage, the `Sparki.h` file and the various `SparkiLib*.c` files
support most of the parts of the Sparki robot *except*:

* Accelerometer
* Magnometer
* Buzzer
* `Radio.h`/NRF24L01+ radio. (Not used by`Sparki.cpp`).
* Serial communication (by the USB connection, or by Bluetooth).

(Not implemented because the Accelerometer/Magnometer rely on TWI/I2C protocol,
which I've not yet ported; the Buzzer relies on a C++ part of Arduino library).

It seems that [TWI and I2C are
similar](http://electronics.stackexchange.com/questions/50915/what-is-difference-between-i2c-and-twi),
so perhaps the functionality for Accelerometer/Magnometer.
