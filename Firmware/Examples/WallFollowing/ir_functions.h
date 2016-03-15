#ifndef _IR_FUNCTIONS_H
#define _IR_FUNCTIONS_H

uint16_t Calculate_Front_IR_Value(uint8_t inches);
uint16_t Calculate_Back_IR_Value(uint8_t inches);
uint16_t Calculate_Left_IR_Value(uint8_t inches);
uint16_t Calculate_Right_IR_Value(uint8_t inches);
double Calculate_Front_Inch_Value(double IR_Value);
double Calculate_Back_Inch_Value(double IR_Value);
double Calculate_Left_Inch_Value(double IR_Value);
double Calculate_Right_Inch_Value(double IR_Value);

#endif