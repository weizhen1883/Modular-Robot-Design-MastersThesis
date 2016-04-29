from periphery import GPIO
from periphery import SPI

gpio_out = GPIO(4, "out")
spi = SPI("/dev/spidev0.0", 0, 10000)

gpio_out.write(True)

data_out = [ord('M'), ord(':'), ord('G'), ord('O'), 30, ord('1'), 30, ord('1'), ord(' '), ord('\n')]
data_in = spi.transfer(data_out)

print("shifted out %-10s" % (''.join(chr(c) for c in data_out),))
print("shifted in  %-10s" % (''.join(chr(c) for c in data_in),))

spi.close()

gpio_out.write(False)

gpio_out.close()