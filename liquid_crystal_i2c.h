/*
 * LiquidCrystalI2CDevice.h
 *
 * Created: 12/9/2018 11:12:08 AM
 *  Author: Barrett
 */ 


#ifndef LIQUIDCRYSTALI2CDEVICE_H_
#define LIQUIDCRYSTALI2CDEVICE_H_

#include <avr/io.h>
#include <stdint.h>

#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define LCD_ENABLE_BIT 0b00000100  // Enable bit
#define LCD_READ_WRITE_BIT 0b00000010  // Read/Write bit
#define LCD_REGISTER_SELECT_BIT 0b00000001  // Register select bit

typedef struct LiquidCrystalDevice_t {
	uint8_t Address;
	uint8_t Columns;
	uint8_t Rows;
	uint8_t Backlight;
	uint8_t DisplayFunction;
	uint8_t DisplayControl;
	uint8_t DisplayMode;
} LiquidCrystalDevice_t;

struct LiquidCrystalDevice_t lq_init(uint8_t address, uint8_t columns, uint8_t rows, uint8_t dotSize);

void lq_clear(struct LiquidCrystalDevice_t* device);

void lq_setCursor(struct LiquidCrystalDevice_t* device, uint8_t row, uint8_t column);

void lq_returnHome(struct LiquidCrystalDevice_t* device);

void lq_turnOnBacklight(struct LiquidCrystalDevice_t* device);

void lq_turnOffBacklight(struct LiquidCrystalDevice_t* device);

void lq_print(struct LiquidCrystalDevice_t* device, char* value);

void lq_turnOnDisplay(struct LiquidCrystalDevice_t* device);

void lq_turnOffDisplay(struct LiquidCrystalDevice_t* device);

void lq_turnOnCursor(struct LiquidCrystalDevice_t* device);

void lq_turnOffCursor(struct LiquidCrystalDevice_t* device);

void lq_turnOnBlink(struct LiquidCrystalDevice_t* device);

void lq_turnOffBlink(struct LiquidCrystalDevice_t* device);

void lq_scrollDisplayLeft(struct LiquidCrystalDevice_t* device);

void lq_scrollDisplayRight(struct LiquidCrystalDevice_t* device);

void lq_leftToRight(struct LiquidCrystalDevice_t* device);

void lq_rightToLeft(struct LiquidCrystalDevice_t* device);

void lq_turnOnAutoscroll(struct LiquidCrystalDevice_t* device);

void lq_turnOffAutoscroll(struct LiquidCrystalDevice_t* device);

void lq_createChar(struct LiquidCrystalDevice_t* device, uint8_t slot, uint8_t charmap[8]);


void lq_sendCommand(struct LiquidCrystalDevice_t* device, uint8_t command);

void lq_writeDeviceByte(struct LiquidCrystalDevice_t* device, uint8_t value, uint8_t mode);

void lq_writeDevice4Bits(struct LiquidCrystalDevice_t* device, uint8_t value);

void lq_writeDevicePulse(struct LiquidCrystalDevice_t* device, uint8_t value);

void lq_transmitI2C(struct LiquidCrystalDevice_t* device, uint8_t value);

#endif /* LIQUIDCRYSTALI2CDEVICE_H_ */