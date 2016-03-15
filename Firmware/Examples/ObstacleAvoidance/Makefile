CC=gcc
CFLAGS=-I/home/pi/c-periphery-master/src
LIBS = -lm
periphery = /home/pi/c-periphery-master/periphery.a

FILE_SBO = stop_before_obstacle.c stepper_motor_api.c ir_functions.c ir_sensor_api.c
FILE_SA = shy_avoid.c stepper_motor_api.c ir_functions.c ir_sensor_api.c
FILE_OA = main.c stepper_motor_api.c ir_functions.c ir_sensor_api.c

TARGET = StopBeforeObstacle ShyAvoid ObstacleAvoid

all: $(TARGET)

StopBeforeObstacle: $(FILE_SBO)
	$(CC) -o $@ $^ $(periphery) $(CFLAGS) $(LIBS)

ShyAvoid: $(FILE_SA)
	$(CC) -o $@ $^ $(periphery) $(CFLAGS) $(LIBS)

ObstacleAvoid: $(FILE_OA)
	$(CC) -o $@ $^ $(periphery) $(CFLAGS) $(LIBS)

clean:
	rm $(TARGET) 
