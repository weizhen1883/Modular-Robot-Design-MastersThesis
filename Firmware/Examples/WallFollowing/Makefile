CC=gcc
CFLAGS=-I/home/pi/c-periphery-master/src
LIBS = -lm
periphery = /home/pi/c-periphery-master/periphery.a

FILE_WF = main.c stepper_motor_api.c ir_functions.c ir_sensor_api.c

TARGET = WallFollowing

all: $(TARGET)

WallFollowing: $(FILE_WF)
	$(CC) -o $@ $^ $(periphery) $(CFLAGS) $(LIBS)

clean:
	rm $(TARGET) 
