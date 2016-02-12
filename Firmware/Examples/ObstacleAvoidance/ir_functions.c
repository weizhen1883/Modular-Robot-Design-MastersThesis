#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "ir_functions.h"

uint16_t Calculate_Front_IR_Value(uint8_t inches) {
	return (uint16_t)(502.62 * pow(inches, -0.775));
}

uint16_t Calculate_Back_IR_Value(uint8_t inches) {
	return (uint16_t)(518.82 * pow(inches, -0.822));
}

uint16_t Calculate_Left_IR_Value(uint8_t inches) {
	return (uint16_t)(692.01 * pow(inches, -1.018));
}

uint16_t Calculate_Right_IR_Value(uint8_t inches) {
	return (uint16_t)(936.28 * pow(inches, -0.779));
}