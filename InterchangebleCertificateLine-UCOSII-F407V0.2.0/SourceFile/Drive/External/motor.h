#ifndef __DC_motor_H
#define __DC_motor_H

#include "Resources.h"
void controlChamferMotor(CutMotorns_Struct * Name, uint16_t Time, uint8_t Function);
uint8_t readChamferMotor(CutMotorns_Struct * Name);
void runChamferMotor(CutMotorns_Struct * Name);

void controlFlotasmCutMotor(CutMotorns_Struct * Name, uint16_t Time, uint8_t Function);
uint8_t readFlotasmCutMotor(CutMotorns_Struct * Name);
void runFlotasmCutMotor(CutMotorns_Struct * Name);

void controlStampACMotor(StampMotorns_Struct *Name,uint32_t Time,uint8_t Function);
void runStampACMotor(StampMotorns_Struct *Name);
uint8_t readStampACMotor(StampMotorns_Struct *Name);

void controlPagingConveyerMotor(PagingMotorns_Struct * Name,uint16_t Time,uint8_t Function);
uint8_t readPagingConveyerMotor(PagingMotorns_Struct *Name);
void runPagingConveyerMotor(PagingMotorns_Struct * Name);

void controlHobMotor(PagingMotorns_Struct * Name,uint16_t Time,uint8_t Function);
uint8_t readHobMotor(PagingMotorns_Struct *Name);
void runHobMotor(PagingMotorns_Struct * Name);

void initCutAllMotor(CutMotorns_Struct * Name);

#endif
