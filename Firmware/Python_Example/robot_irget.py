from periphery import GPIO
from periphery import SPI
import time

gpio_out = GPIO(17, "out")
spi = SPI("/dev/spidev0.0", 0, 10000)

gpio_out.write(True)

data_out = [ord('S'), ord(':'), ord('P'), ord('R'), ord('E'), ord(' '), ord('I'), ord('R'), ord(' '), ord('\n')]
data_in = spi.transfer(data_out)

print("shifted out %-10s" % (''.join(chr(c) for c in data_out),))
print("shifted in  %-10s" % (''.join(chr(c) for c in data_in),))

time.sleep(0.1)

data_out = [ord('S'), ord(':'), ord('G'), ord('E'), ord('T'), ord(' '), ord('I'), ord('R'), ord(' '), ord('\n')]
data_in = spi.transfer(data_out)

dataFront = ((data_in[2]-1) << 8) | (data_in[3]);
dataBack = ((data_in[4]-1) << 8) | (data_in[5]);
dataLeft = ((data_in[6]-1) << 8) | (data_in[7]);
dataRight = ((data_in[8]-1) << 8) | (data_in[9]);

message_out = [chr(data_in[0]), chr(data_in[1]), " F=", str(dataFront), " B=", str(dataBack), " L=", str(dataLeft), " R=", str(dataRight)]

print("shifted out %-10s" % (''.join(chr(c) for c in data_out),))
print("shifted in  %s" % (''.join(m for m in message_out),))

spi.close()

gpio_out.write(False)

gpio_out.close()