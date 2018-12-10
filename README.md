# Liquid Crystal I2C library for AVR
Port of Arduino Liquid Crystal I2C library for HD44780 (or a compatible) displays from Frank de Brabander (https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library) for ATmega Controller (e.g. 328P).

## Getting Started

Copy both `liquid_crystal_i2c.c` and `liquid_crystal_i2c.h` to your project and just start using it.
**Note:** this library uses custom I2C AVR implementation ([cheprogrammer/i2c_avr](https://github.com/cheprogrammer/i2c_avr "My I2C implementation")). It is easy to get rid of this library and use your own I2C routine.

### Example of usage

At the begining of programm initialize your i2c peripheral and call the Liquid Crystal I2C initialization routine by using the following method:

```C
lq_init(0x27, 20, 4, LCD_5x8DOTS);
```

This function accepts I2C shield address for LCD display, number of columns, rows and dot size (defined `LCD_5x8DOTS`, `LCD_5x10DOTS`). After performing all initialization routine, this function returns instance of `LiquidCrystalDevice_t` structure. This structure should be stored somewhere in programm because it contains required information for evaluating future operations.

Here is the definition of this structure:
```C
typedef struct LiquidCrystalDevice_t {
	uint8_t Address;
	uint8_t Columns;
	uint8_t Rows;
	uint8_t Backlight;
	uint8_t DisplayFunction;
	uint8_t DisplayControl;
	uint8_t DisplayMode;
} LiquidCrystalDevice_t;
```

Here is the example of initialization and printing some text:

```C
LiquidCrystalDevice_t device = lq_init(0x27, 20, 4, LCD_5x8DOTS); // intialize 4-lines display

lq_turnOnBacklight(&device); // simply turning on the backlight

lq_print(&device, "Hello world!");
lq_setCursor(&device, 1, 0); // moving cursor to the next line
lq_print(&device, "How are you?");
```

### I2C Error handling
Current version of library does not support I2C error handling. It will be added in the future (maybe).

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
