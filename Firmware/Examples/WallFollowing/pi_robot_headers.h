#ifndef _PI_ROBOT_HEADERS_H
#define _PI_ROBOT_HEADERS_H

struct Motors {
	uint8_t speed_l;
	uint8_t speed_r;
	uint8_t direction_l;
	uint8_t direction_r;
};

struct SensorLimits {
	uint16_t frontLimit;
	uint16_t backLimit;
	uint16_t leftLimit;
	uint16_t rightLimit;
};



#endif