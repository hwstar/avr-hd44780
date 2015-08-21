# avr-hd44780

## Description

This is an avr-gcc library for the HD44780 character LCD display. Forked 
from [aostanin/avr-hd44780](https://github.com/aostanin/avr-hd44780) 
and modified to suit my requirements.

## Usage

* Modify `pins.h` to set the I/O up the way you like it. 

The control signals must all be on one port.
The data signals must all be on consecutive pins and all on one port.

Different ports may be used for the control and data pins.

The LCD is operated in 4 bit mode to conserve I/O pins. A total of 7 
pins are required.

* Edit `lcd.h` to change the number of row and column counts for your
particular LCD.


* See `lcd.c` for the callable methods

* Include all 3 files in your project.


## License

Licensed under the GNU General Public License, Version 2.0.

You may find a copy of the license at

```
http://www.gnu.org/licenses/gpl-2.0.html
```

## References

- [SC1602BS Character Display Module](http://akizukidenshi.com/catalog/g/gP-00040/)
- [Arduino's LiquidCrystal library source](https://code.google.com/p/arduino/source/browse/trunk/libraries/LiquidCrystal)
- [https://github.com/aostanin/avr-hd44780]
