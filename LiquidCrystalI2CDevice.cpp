/*
* LiquidCrystalI2CDevice.c
*
* Created: 12/9/2018 11:35:38 AM
*  Author: Barrett
*/

#include "I2CMasterDevice.h"
#include "LiquidCrystalI2CDevice.h"

#include <avr/io.h>
#include <util/delay.h>

LiquidCrystalDevice_t lq_init(uint8_t address, uint8_t columns, uint8_t rows, uint8_t dotSize)
{
	LiquidCrystalDevice_t device;

	device.Address = address;
	device.Columns = columns;
	device.Rows = rows;
	device.Backlight = LCD_NOBACKLIGHT;
	device.DisplayFunction = LCD_4BITMODE | LCD_1LINE | dotSize;
	device.DisplayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;

	// Initialize to default text direction (for roman languages)
	device.DisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	device.Row = 0;
	device.Column = 0;

	if (rows > 1) {
		device.DisplayFunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotSize != 0) && (rows == 1)) {
		device.DisplayFunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	_delay_ms(100);
	
	// Now we pull both RS and R/W low to begin commands
	lq_transmitToDisplay(&device, LCD_NOBACKLIGHT);	// reset expanderand turn backlight off (Bit 8 =1)
	_delay_ms(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
	
	// we start in 8bit mode, try to set 4 bit mode
	lq_transmitToDisplay(&device, 0x03 << 4);
	_delay_us(4500); // wait min 4.1ms
	
	// second try
	lq_send4Bits(&device, 0x03 << 4);
	_delay_us(4500); // wait min 4.1ms
	
	// third go!
	lq_send4Bits(&device, 0x03 << 4);
	_delay_us(150); // wait min 150 mics
	
	// finally, set to 4-bit interface
	lq_send4Bits(&device, 0x02 << 4);

	// set # lines, font size, etc.
	lq_sendCommand(&device, LCD_FUNCTIONSET | device.DisplayFunction);
	
	// turn the display on with no cursor or blinking default
	lq_turnOnDisplay(&device);
	
	// clear it off
	lq_clear(&device);
	
	// set the entry mode
	lq_sendCommand(&device, LCD_ENTRYMODESET | device.DisplayMode);
	
	lq_returnHome(&device);

	return device;
};

void lq_print(struct LiquidCrystalDevice_t* device, char * value)
{
	char letter = *value;

	while(letter != 0x00)
	{
		lq_sendByte(device, letter, LCD_REGISTER_SELECT_BIT);
		letter = *(++value);
	}
};

void lq_turnOnBacklight(struct LiquidCrystalDevice_t* device)
{
	device->Backlight = LCD_BACKLIGHT;
	lq_transmitToDisplay(device, 0);
}

void lq_turnOffBacklight(struct LiquidCrystalDevice_t* device)
{
	device->Backlight = LCD_NOBACKLIGHT;
	lq_transmitToDisplay(device, 0);
}

void lq_turnOnDisplay(LiquidCrystalDevice_t* device)
{
	device->DisplayControl |= LCD_DISPLAYON;
	lq_sendCommand(device, LCD_DISPLAYCONTROL | device->DisplayControl);
};

void lq_clear(LiquidCrystalDevice_t* device)
{
	lq_sendCommand(device, LCD_CLEARDISPLAY); // clear display, set cursor position to zero
	_delay_us(2000);  // this command takes a long time!

	lq_setCursor(device, 0, 0);
}

void lq_setCursor(LiquidCrystalDevice_t* device, uint8_t row, uint8_t column)
{
	uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

	lq_sendCommand(device, LCD_SETDDRAMADDR | (column + row_offsets[row]));
}

void lq_returnHome(LiquidCrystalDevice_t* device)
{
	lq_sendCommand(device, LCD_RETURNHOME);  // set cursor position to zero
	_delay_us(2000);  // this command takes a long time!
}

void lq_sendCommand(LiquidCrystalDevice_t* device, uint8_t command)
{
	lq_sendByte(device, command, 0);
}

void lq_sendByte(LiquidCrystalDevice_t* device, uint8_t value, uint8_t mode)
{
	uint8_t highnib= value & 0xf0;
	uint8_t lownib= (value<<4) & 0xf0;

	lq_send4Bits(device, highnib | mode);
	lq_send4Bits(device, lownib | mode);
};

void lq_send4Bits(LiquidCrystalDevice_t* device, uint8_t value)
{
	lq_transmitToDisplay(device, value);
	lq_sendPulse(device, value);
};

void lq_sendPulse(LiquidCrystalDevice_t* device, uint8_t value)
{
	lq_transmitToDisplay(device, value | LCD_ENABLE_BIT);
	_delay_us(2);

	lq_transmitToDisplay(device, value & ~LCD_ENABLE_BIT);
	_delay_us(50);
};

void lq_transmitToDisplay(LiquidCrystalDevice_t* device, uint8_t value)
{
	i2c_master_sendByte(device->Address, value | device->Backlight);
};