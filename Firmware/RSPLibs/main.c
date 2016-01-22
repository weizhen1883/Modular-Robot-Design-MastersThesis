#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "stepper_motor_api.h"

int main(int argc, char const *argv[])
{
	robotMotorModuleInit(23);
	printf("%s\n", argv[1]);
	if (!strcmp(argv[1],"go")) {
		robotGo(50,0,50,0);
	} else if (!strcmp(argv[1],"stop")) {
		robotStop();
	} else if (!strcmp(argv[1], "get")) {
		robotGet();
	}
	return 0;
}