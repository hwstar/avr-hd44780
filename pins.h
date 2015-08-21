//
//		pins.h
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

#ifndef PINS_H
#define PINS_H

/*
 * This file contains port and pin definitions for various devices
 * which can be attached the the AVR microcontroller
 */
 

/*
 * HD44780 Character LCD
 */

#define CHAR_LCD_CTRL_DDR DDRD
#define CHAR_LCD_CTRL PORTD
	#define CHAR_LCDE 4
	#define CHAR_LCDRW 5
	#define CHAR_LCDRS 6

#define CHAR_LCD_DATA_DDR DDRB
#define CHAR_LCD_DATA PORTB
	#define CHAR_LCD_DATA_SHIFT 0
	#define CHAR_LCD_DATA_MASK 0x0F

#endif

