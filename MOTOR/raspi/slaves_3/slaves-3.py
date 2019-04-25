import smbus
import time
# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address1 = 0x04
address2 = 0x05
address3 = 0x06

def writeNumber(address, value):
    bus.write_byte(address, value)
    # bus.write_byte_data(address, 0, value)
    return -1

def readNumber(address):
    number = bus.read_byte(address)
    # number = bus.read_byte_data(address, 1)
    return number

def encodeAddress(argument):
    switcher = {
        4: 0x04,
        5: 0x05,
        6: 0x06,
    }
    return switcher.get(argument, 0x04)
    
while True:
    var = input("Enter 1 to 9: ")
    if not var:
        continue
    
    
    writeNumber(encodeAddress(var), var)
    print "RPI: Hi Arduino, I sent you ", var
    # sleep one second
    time.sleep(1)

    number = readNumber(encodeAddress(var))
    print "Arduino: Hey RPI, I received a digit ", number
    print

