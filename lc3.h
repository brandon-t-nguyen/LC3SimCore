//
// Created by Brandon on 8/17/2015.
//

#ifndef LC3SIMCORE_LC3_H
#define LC3SIMCORE_LC3_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#define GPR_NUM 8

#define OP_BR   0x0
#define OP_ADD  0x1
#define OP_LD   0x2
#define OP_ST   0x3
#define OP_JSR  0x4
#define OP_AND  0x5
#define OP_LDR  0x6
#define OP_STR  0x7
#define OP_RTI  0x8
#define OP_NOT  0x9
#define OP_LDI  0xA
#define OP_STI  0xB
#define OP_JMP  0xC
#define OP_RESV 0xD
#define OP_LEA  0xE
#define OP_TRAP 0xF

//Memory mapped io addrs
//80x24 terminal
#define KBSR    0xFE00
#define KBDR    0xFE02
#define DSR     0xFE04
#define DDR     0xFE06
#define MCR     0xFFFE
#define IVT     0x0100

//Interrupt vector
#define IV_ILL   0x00
#define IV_EXC   0x01
#define IV_KB    0x80
#define IV_HD    0x82
//New io device:
#define HDSR    0xFE08  //Hard disk status register
#define HDBR    0xFE0A  //Hard disk block register
#define HDDR    0xFE0C  //Hard disk data register

typedef struct lc3_str {
    uint16_t psr;           //process status register
    uint16_t pc;            //program counter
    uint16_t ir;           //Memory address register
    uint16_t mar;           //Memory address register
    uint16_t mdr;           //Memory data register
    uint8_t intLines[32];   //A bit for each possible interrupt
    uint16_t registers[8];  //General purpose regs
    //struct Memory_str memory;      //Memory struct
    uint16_t mem[0x10000];
    /*
    uint8_t* disk;          //Attach a disk to the LC3 when wanted
    uint8_t diskStatus;     //How many cycles before disk read is complete
    uint8_t vram[1920];     //Memory for the text display. Not accessible to user.
    uint8_t cursorX;
    uint8_t cursorY;
    */
} lc3;


typedef enum CycleReturn_e {
    SUCCESS,
    HALT,
    INTERRUPT,
} CycleReturn;

void lc3_init(lc3 *ptr);

enum CycleReturn_e lc3_cycle(lc3 *ptr);

void lc3_ioHandle(lc3 *ptr);

//Returns 1 if interrupted
uint8_t lc3_checkInterrupts(lc3 *ptr);

//IO handlers
void lc3_keyHandle(lc3 *ptr, uint8_t key);      //Function to deal with keyboard
void lc3_disHandle(lc3 *ptr);

void lc3_diskHandle(lc3 *ptr);

void lc3_interrupt(lc3 *ptr, uint8_t intNum, uint8_t priority);

//Helper functions
//Sign extend. Bitnum is the bit number
int16_t lc3_SEXT(uint16_t input, uint8_t bitnum);

//Set condition codes. Provide it the number that will determine it
void lc3_setcc(lc3 *ptr, uint16_t num);

//Returns 1 or 0 for the particular interrupt line
uint8_t lc3_getIntLine(lc3 *ptr, uint8_t intLine);

//Sets the provided interrupt line
void lc3_setIntLine(lc3 *ptr, uint8_t intLine);

//Clears the provided interrupt line
void lc3_clearIntLine(lc3 *ptr, uint8_t intLine);

//Returns 0 if machine isn't on
uint8_t lc3_checkMachine(lc3 *ptr);


//Classic loading scheme: first word is entry point
//COMMENTED OUT BECAUSE IT'S NOT THE PROCESSOR'S JOB TO LOAD CODE THROUGH THE HOST SYSTEM
//void lc3_loadObj(lc3 *ptr, obj_file obj);

//Inline functions for each instruction
//Branch
void lc3_BR(lc3 *ptr);

//Add and And; they do the same thing except operation
void lc3_ANDD(lc3 *ptr);

//Load/Store
void lc3_LS(lc3 *ptr);

//JSR and JSRR
void lc3_JSR(lc3 *ptr);

//Load/Store register
void lc3_LSR(lc3 *ptr);

//Return from interrupt
void lc3_RTI(lc3 *ptr);

//NOT
void lc3_NOT(lc3 *ptr);

//Load/Store indirect
void lc3_LSI(lc3 *ptr);

//Jump and Jump Register
void lc3_JMP(lc3 *ptr);

//Reserved
void lc3_RESV(lc3 *ptr);

//Load Effective Address
void lc3_LEA(lc3 *ptr);

//Trap
void lc3_TRAP(lc3 *ptr);

//All memory access instructions undergo the same sequence after the MAR is calculated
void lc3_MEM(lc3 *ptr, uint16_t inst, uint16_t *regptr);

#ifdef __cplusplus
}
#endif


#endif //LC3SIMCORE_LC3_H
