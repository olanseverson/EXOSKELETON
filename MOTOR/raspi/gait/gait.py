from smbus2 import SMBusWrapper, i2c_msg
import time

def readNumber(address, length):
    with SMBusWrapper(1) as bus:
        msg = i2c_msg.read(address, length)
        bus.i2c_rdwr(msg)
        return msg
        
def writeNumber(address, data):
    with SMBusWrapper(1) as bus:
        msg = i2c_msg.write(address, data)
        bus.i2c_rdwr(msg)
    return -1

def writeWord(address, word_data):
    data = []
    data.append((word_data&0xff00)>>8)
    data.append(word_data&0x00ff)
    writeNumber(address, data)
    return -1
    

while True:
    address = 0x20
    with SMBusWrapper(1) as bus:
        
        # Write some bytes to address 0x10
        data = input("enter in array formatted: ")
        writeWord(address, data)
        
        # Read 64 bytes from address 0x10
        msg = readNumber(address, 1)
        for value in msg:
            print(value)
            
        # show structure of object
        #~ l = dir(msg) 
        #~ print l
    time.sleep(1)
    
#~ import smbus
#~ import time
#~ import numpy as np
#~ # for RPI version 1, use "bus = smbus.SMBus(0)"
#~ bus = smbus.SMBus(1)

#~ # This is the address we setup in the Arduino Program
#~ address1 = 0x04
#~ address2 = 0x05
#~ address3 = 0x06

#~ def writeNumber(address, value):
    #~ #bus.write_byte(address, value)
    #~ #bus.write_block_data(address, 99, value)
    #~ bus.write_i2c_block_data(address, 99, value)
    #~ return -1

#~ def readNumber(address):
    #~ number = bus.read_byte(address)
    #~ #number = bus.read_byte_data(address, 99)
    #~ #number = bus.read_i2c_block_data(address, 99)
    #~ return number

#~ def encodeAddress(argument):
    #~ switcher = {
        #~ 4: address1,
        #~ 5: address2,    
        #~ 6: address3,
    #~ }
    #~ return switcher.get(argument, address1)
    
#~ writeNumber(0x05, [500/4])
#~ while True:
    #~ var = input("Enter 1 to 9: ")
    #~ for i in range (len(var)):
        #~ var[i] = var[i]/4
    #~ print(type(var), var)
    #~ if not var:
        #~ continue
    
    
    #~ writeNumber(0x05, var)
    #~ print "RPI: Hi Arduino, I sent you ", var
    #~ # sleep one second
    #~ time.sleep(1)

    #~ number = readNumber(0x05)
    #~ print "Arduino: Hey RPI, I received a digit ", number
    #~ print
