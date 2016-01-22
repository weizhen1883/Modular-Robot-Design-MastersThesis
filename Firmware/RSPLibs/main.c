#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "stepper_motor_api.h"

int int main(int argc, char const *argv[])
{
	if (strcmp(argv[1],"go")) {
		robotGo(50,0,50,0);
	} else if (strcmp(argv[1],"stop")) {
		robotStop();
	}
	return 0;
}