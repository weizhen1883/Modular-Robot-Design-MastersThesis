CC=gcc
CFLAGS=-I/home/pi/c-periphery-master/src
LIBS = -lm
periphery = /home/pi/c-periphery-master/periphery.a

FILE = main.c stepper_motor_api.c ir_sensor_api.c

TARGET = robot

all: $(TARGET)

robot: $(FILE)
	$(CC) -o $@ $^ $(periphery) $(CFLAGS) $(LIBS)

clean:
	rm $(TARGET) 
