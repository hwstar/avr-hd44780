
#include <avr/io.h>
#include <stdarg.h>
#include <stdio.h>
#include <util/delay.h>

#include "lcd.h"
#include "pins.h"
//
//		lcd.c
//
//		Copyright 2015 Stephen Rodgers
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//      
//

#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE    0x08
#define LCD_1LINE    0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS  0x00

static void lcd_send(uint8_t value, uint8_t mode);
static void lcd_write_nibble(uint8_t nibble);

static uint8_t lcd_displayparams;
static char lcd_buffer[LCD_COL_COUNT + 1];


/*
 * Send a command to the LCD
 */
 
static void lcd_command(uint8_t command)
{
	lcd_send(command, 0);
}

/*
 * Send a character to the LCD
 */

static void lcd_write(uint8_t value)
{
	lcd_send(value, 1);
}

/*
 * Send a byte of data to the LCD in a given mode
 */

static void lcd_send(uint8_t value, uint8_t mode)
{
	// Set  or clear RS bit 
	CHAR_LCD_CTRL &= ~_BV(CHAR_LCDRS);
	if(mode)
		CHAR_LCD_CTRL |= _BV(CHAR_LCDRS);
	
	// Send the nibbles
	lcd_write_nibble(value >> 4);
	lcd_write_nibble(value);
}

/*
 * Write a nibble to the LCD data port
 */
 
static void lcd_write_nibble(uint8_t nibble)
{
	// Clear old bits
	CHAR_LCD_DATA &= ~CHAR_LCD_DATA_MASK;
	
	// Pre-shift nibble if required
	#if CHAR_LCD_DATA_SHIFT > 0
	nibble <<= CHAR_LCD_DATA_SHIFT
	nibble &= ~CHAR_LCD_DATA_MASK;
	#endif
	

	// OR in new bits
	CHAR_LCD_DATA |= nibble;

	// Strobe the E signal
	CHAR_LCD_CTRL |= _BV(CHAR_LCDE);
	CHAR_LCD_CTRL &= ~(_BV(CHAR_LCDE));

	_delay_ms(0.04);
}

/*
 * Initialize LCD I/O pins, initialize 4 bit LCD mode
 * 
 */

void lcd_init(void)
{

	// Configure pins as output
	CHAR_LCD_CTRL_DDR |= _BV(CHAR_LCDE) | _BV(CHAR_LCDRS) | _BV(CHAR_LCDRW);
	CHAR_LCD_DATA_DDR |= CHAR_LCD_DATA_MASK;

	// Wait for LCD to become ready (docs say 15ms+)
	_delay_ms(15);

	// Set control signals low
	CHAR_LCD_CTRL &= ~(_BV(CHAR_LCDE) | _BV(CHAR_LCDRS) | _BV(CHAR_LCDRW));

	_delay_ms(4.1);

	lcd_write_nibble(0x03); // Switch to 4 bit mode
	_delay_ms(4.1);

	lcd_write_nibble(0x03); // 2nd time
	_delay_ms(4.1);

	lcd_write_nibble(0x03); // 3rd time
	_delay_ms(4.1);

	lcd_write_nibble(0x02); 

	lcd_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);

	lcd_displayparams = LCD_CURSOROFF | LCD_BLINKOFF;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
	
	lcd_on();
	lcd_clear();
}

/*
 * Turn LCD on
 */

void lcd_on(void)
{
  lcd_displayparams |= LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

/*
 * Turn LCD off
 */

void lcd_off(void)
{
	lcd_displayparams &= ~LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

/*
 * Clear LCD
 */

void lcd_clear(void) {
	lcd_command(LCD_CLEARDISPLAY);
	_delay_ms(2);
}

/*
 * Return home
 */

void lcd_return_home(void)
{
	lcd_command(LCD_RETURNHOME);
	_delay_ms(2);
}

/*
 * Enable blinking
 */

void lcd_enable_blinking(void) {
	lcd_displayparams |= LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

/*
 * Disable blinking
 */
 

void lcd_disable_blinking(void)
{
	lcd_displayparams &= ~LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

/*
 * Enable cursor
 */

void lcd_enable_cursor(void)
{
	lcd_displayparams |= LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

/*
 * Disable cursor
 */
 
void lcd_disable_cursor(void)
{
	lcd_displayparams &= ~LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

/*
 * Scroll left
 */
 
void lcd_scroll_left(void) {
	lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

/*
 * Scroll right
 */


void lcd_scroll_right(void)
{
	lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/*
 * Set left to right entry mode
 */

void lcd_set_left_to_right(void)
{
	lcd_displayparams |= LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

/*
 * Set right to left entry mode
 */

void lcd_set_right_to_left(void)
{
	lcd_displayparams &= ~LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

/*
 * Enable auto scroll
 */
 
void lcd_enable_autoscroll(void)
{
	lcd_displayparams |= LCD_ENTRYSHIFTINCREMENT;
	lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

/*
 * Disable auto scroll
 */
 
void lcd_disable_autoscroll(void)
{
	lcd_displayparams &= ~LCD_ENTRYSHIFTINCREMENT;
	lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

/*
 * Create a custom character
 */
 

void lcd_create_char(uint8_t location, uint8_t *charmap)
{
	int i;
	
	lcd_command(LCD_SETCGRAMADDR | ((location & 0x7) << 3));
	for (i = 0; i < 8; i++){
		lcd_write(charmap[i]);
	}
}

/*
 * Set cursor positon
 * 
 * Specify row, column
 */
 
void lcd_set_cursor(uint8_t row, uint8_t col)
{
	static uint8_t offsets[] = { 0x00, 0x40, 0x14, 0x54 };

	if (row > 1){
		row = 1;
	}

	lcd_command(LCD_SETDDRAMADDR | (col + offsets[row]));
}

/*
 * Positional putc
 */

void lcd_pos_putc(uint8_t row, uint8_t col, char c)
{
	lcd_set_cursor(row, col);
	lcd_write(c);
}

/*
 * Positional puts
 *
 * Specify row, column, and string to print
 */

void lcd_pos_puts(uint8_t row, uint8_t col, char *string)
{
	lcd_set_cursor(row, col);
	char *p;
	for (p = string; *p; p++){
		lcd_write(*p);
	}
}

/*
 * Positional printf
 * 
 * Specify row, column, format, and optional arguments
 */
 
void lcd_pos_printf(uint8_t row, uint8_t col, char *format, ...)
{
	va_list args;

	va_start(args, format);
	vsnprintf(lcd_buffer, LCD_COL_COUNT + 1, format, args);
	va_end(args);
	lcd_pos_puts(row, col, lcd_buffer);
}
