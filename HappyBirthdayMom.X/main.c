/* 
 * File:   main.c
 * Author: Pavlo
 *
 * Created on October 12, 2013, 11:36 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "I2CdsPIC.h"

//I2C Addresses
#define COLUMN1 00
#define COLUMN2 02
#define COLUMN3 04
#define COLUMN4 06
#define COLUMN5 08

#define PRODUCTION

_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
_FICD(JTAGEN_OFF & ICS_PGD1);

//ClockInit needs to be updated for 40 MIPS.  Really.
void ClockInit();
void TimersInit();

// HEY MOM, HAPPY 50th!
uint8_t Message[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, //
	0x7F, 0x08, 0x08, 0x08, 0x7F, //H
	0x7F, 0x49, 0x49, 0x49, 0x41, //E
	0x03, 0x04, 0x78, 0x04, 0x03, //Y
	0x00, 0x00, 0x00, 0x00, 0x00, // 
	0x7F, 0x02, 0x04, 0x02, 0x7F, //M
	0x3E, 0x41, 0x41, 0x41, 0x3E, //O
	0x7F, 0x02, 0x04, 0x02, 0x7F, //M
	0x00, 0x00, 0x00, 0x00, 0x00, //
	0x7F, 0x08, 0x08, 0x08, 0x7F, //H
	0x7E, 0x11, 0x11, 0x11, 0x7E, //A
	0x7F, 0x09, 0x09, 0x09, 0x06, //P
	0x7F, 0x09, 0x09, 0x09, 0x06, //P
	0x03, 0x04, 0x78, 0x04, 0x03, //Y
	0x27, 0x45, 0x45, 0x45, 0x39, //
	0x3E, 0x51, 0x49, 0x45, 0x3E, //5
	0x04, 0x3F, 0x44, 0x40, 0x20, //0
	0x7F, 0x08, 0x04, 0x04, 0x78, //t
	0x00, 0x00, 0x5F, 0x00, 0x00, //h
	0x00, 0x00, 0x00, 0x00, 0x00 //!
};

#ifndef PRODUCTION
uint16_t ASCII[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, // (space) 4
	0x00, 0x00, 0x5F, 0x00, 0x00, // ! 9
	0x00, 0x07, 0x00, 0x07, 0x00, // " 14
	0x14, 0x7F, 0x14, 0x7F, 0x14, // # 19
	0x24, 0x2A, 0x7F, 0x2A, 0x12, // $ 24
	0x23, 0x13, 0x08, 0x64, 0x62, // % 29
	0x36, 0x49, 0x55, 0x22, 0x50, // & 34
	0x00, 0x05, 0x03, 0x00, 0x00, // ' 39
	0x00, 0x1C, 0x22, 0x41, 0x00, // ( 44
	0x00, 0x41, 0x22, 0x1C, 0x00, // ) 49
	0x08, 0x2A, 0x1C, 0x2A, 0x08, // * 54
	0x08, 0x08, 0x3E, 0x08, 0x08, // + 59
	0x00, 0x50, 0x30, 0x00, 0x00, // , 64
	0x08, 0x08, 0x08, 0x08, 0x08, // - 69
	0x00, 0x60, 0x60, 0x00, 0x00, // . 74
	0x20, 0x10, 0x08, 0x04, 0x02, // / 79
	0x3E, 0x51, 0x49, 0x45, 0x3E, // 0 84
	0x00, 0x42, 0x7F, 0x40, 0x00, // 1 89
	0x42, 0x61, 0x51, 0x49, 0x46, // 2 94
	0x21, 0x41, 0x45, 0x4B, 0x31, // 3 99
	0x18, 0x14, 0x12, 0x7F, 0x10, // 4 104
	0x27, 0x45, 0x45, 0x45, 0x39, // 5 109
	0x3C, 0x4A, 0x49, 0x49, 0x30, // 6 114
	0x01, 0x71, 0x09, 0x05, 0x03, // 7 119
	0x36, 0x49, 0x49, 0x49, 0x36, // 8 124
	0x06, 0x49, 0x49, 0x29, 0x1E, // 9 129
	0x00, 0x36, 0x36, 0x00, 0x00, // : 134
	0x00, 0x56, 0x36, 0x00, 0x00, // ; 139
	0x00, 0x08, 0x14, 0x22, 0x41, // < 144
	0x14, 0x14, 0x14, 0x14, 0x14, // = 149
	0x41, 0x22, 0x14, 0x08, 0x00, // > 154
	0x02, 0x01, 0x51, 0x09, 0x06, // ? 159
	0x32, 0x49, 0x79, 0x41, 0x3E, // @ 164
	0x7E, 0x11, 0x11, 0x11, 0x7E, // A 169
	0x7F, 0x49, 0x49, 0x49, 0x36, // B 174
	0x3E, 0x41, 0x41, 0x41, 0x22, // C 179
	0x7F, 0x41, 0x41, 0x22, 0x1C, // D 184
	0x7F, 0x49, 0x49, 0x49, 0x41, // E 189
	0x7F, 0x09, 0x09, 0x01, 0x01, // F 194
	0x3E, 0x41, 0x41, 0x51, 0x32, // G 199
	0x7F, 0x08, 0x08, 0x08, 0x7F, // H 204
	0x00, 0x41, 0x7F, 0x41, 0x00, // I 209
	0x20, 0x40, 0x41, 0x3F, 0x01, // J 214
	0x7F, 0x08, 0x14, 0x22, 0x41, // K 219
	0x7F, 0x40, 0x40, 0x40, 0x40, // L 224
	0x7F, 0x02, 0x04, 0x02, 0x7F, // M 229
	0x7F, 0x04, 0x08, 0x10, 0x7F, // N 234
	0x3E, 0x41, 0x41, 0x41, 0x3E, // O 239
	0x7F, 0x09, 0x09, 0x09, 0x06, // P 244
	0x3E, 0x41, 0x51, 0x21, 0x5E, // Q 249
	0x7F, 0x09, 0x19, 0x29, 0x46, // R 254
	0x46, 0x49, 0x49, 0x49, 0x31, // S 259
	0x01, 0x01, 0x7F, 0x01, 0x01, // T 264
	0x3F, 0x40, 0x40, 0x40, 0x3F, // U 269
	0x1F, 0x20, 0x40, 0x20, 0x1F, // V 274
	0x7F, 0x20, 0x18, 0x20, 0x7F, // W 279
	0x63, 0x14, 0x08, 0x14, 0x63, // X 284
	0x03, 0x04, 0x78, 0x04, 0x03, // Y 289
	0x61, 0x51, 0x49, 0x45, 0x43, // Z 294
	0x00, 0x00, 0x7F, 0x41, 0x41, // [ 299
	0x02, 0x04, 0x08, 0x10, 0x20, // "\" 304
	0x41, 0x41, 0x7F, 0x00, 0x00, // ] 309
	0x04, 0x02, 0x01, 0x02, 0x04, // ^ 314
	0x40, 0x40, 0x40, 0x40, 0x40, // _ 319
	0x00, 0x01, 0x02, 0x04, 0x00, // ` 324
	0x20, 0x54, 0x54, 0x54, 0x78, // a 329
	0x7F, 0x48, 0x44, 0x44, 0x38, // b 334
	0x38, 0x44, 0x44, 0x44, 0x20, // c 339
	0x38, 0x44, 0x44, 0x48, 0x7F, // d 344
	0x38, 0x54, 0x54, 0x54, 0x18, // e 349
	0x08, 0x7E, 0x09, 0x01, 0x02, // f 354
	0x08, 0x14, 0x54, 0x54, 0x3C, // g 359
	0x7F, 0x08, 0x04, 0x04, 0x78, // h 364
	0x00, 0x44, 0x7D, 0x40, 0x00, // i 369
	0x20, 0x40, 0x44, 0x3D, 0x00, // j 374
	0x00, 0x7F, 0x10, 0x28, 0x44, // k 379
	0x00, 0x41, 0x7F, 0x40, 0x00, // l 384
	0x7C, 0x04, 0x18, 0x04, 0x78, // m 389
	0x7C, 0x08, 0x04, 0x04, 0x78, // n 394
	0x38, 0x44, 0x44, 0x44, 0x38, // o 399
	0x7C, 0x14, 0x14, 0x14, 0x08, // p 404
	0x08, 0x14, 0x14, 0x18, 0x7C, // q 409
	0x7C, 0x08, 0x04, 0x04, 0x08, // r 414
	0x48, 0x54, 0x54, 0x54, 0x20, // s 419
	0x04, 0x3F, 0x44, 0x40, 0x20, // t 424
	0x3C, 0x40, 0x40, 0x20, 0x7C, // u 429
	0x1C, 0x20, 0x40, 0x20, 0x1C, // v 434
	0x3C, 0x40, 0x30, 0x40, 0x3C, // w 439
	0x44, 0x28, 0x10, 0x28, 0x44, // x 444
	0x0C, 0x50, 0x50, 0x50, 0x3C, // y 449
	0x44, 0x64, 0x54, 0x4C, 0x44, // z 554
	0x00, 0x08, 0x36, 0x41, 0x00, // { 559
	0x00, 0x00, 0x7F, 0x00, 0x00, // | 564
	0x00, 0x41, 0x36, 0x08, 0x00, // } 569
	0x08, 0x08, 0x2A, 0x1C, 0x08, // -> 574
	0x08, 0x1C, 0x2A, 0x08, 0x08 // <- 579
};
#endif

static int currentMessageIndex = 0;

void main(void)
{
	ClockInit();
	TimersInit();
	I2C_Init(57600);
	//Default register value is equal to 0b1110000 ADx isn't set
	I2C_WriteToReg(0x70, 0x20, 0b00100001);
	I2C_WriteToReg(0x70, 0xA0, 0b10100000);
	I2C_WriteToReg(0x70, 0x80, 0b10000001);

	//This is the command for 8/16 Duty Setup 0xEx where x is 0 - F for 1/16 to 16/16 duty.
	I2C_WriteToReg(0x70, 0xEF, 0xE7);
}

void ClockInit(void)
{
	PLLFBD = 242; // M = 50 MIPS
	CLKDIVbits.PLLPOST = 0;
	CLKDIVbits.PLLPRE = 7;
	OSCTUN = 0;
	RCONbits.SWDTEN = 0;
	__builtin_write_OSCCONH(0x01); // Initiate Clock Switch to Primary (3?)
	__builtin_write_OSCCONL(0x01); // Start clock switching
	while (OSCCONbits.COSC != 0b001); // Wait for Clock switch to occur
	while (OSCCONbits.LOCK != 1) {
	};
}

void TimersInit(void)
{
	T2CONbits.TON = 0;
	T2CONbits.TCS = 0;
	T2CONbits.TGATE = 0;
	T2CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
	TMR2 = 0x00;
	PR2 = 8000; //31250 / 8000 = 3.9 Hz
	IPC1bits.T2IP = 0x01;
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;
	T2CONbits.TON = 1;
}

//Message Length is 20.

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
	if (currentMessageIndex == 96) {
		currentMessageIndex = 0;
	}
	I2C_WriteToReg(0x70, COLUMN1, Message[currentMessageIndex]);
	I2C_WriteToReg(0x70, COLUMN2, Message[currentMessageIndex + 1]);
	I2C_WriteToReg(0x70, COLUMN3, Message[currentMessageIndex + 2]);
	I2C_WriteToReg(0x70, COLUMN4, Message[currentMessageIndex + 3]);
	I2C_WriteToReg(0x70, COLUMN5, Message[currentMessageIndex + 4]);
	I2C_WriteToReg(0x70, 0x80, 0b10000001);
	currentMessageIndex++;
	IFS0bits.T2IF = 0; // Clear Timer1 Interrupt Flag
}