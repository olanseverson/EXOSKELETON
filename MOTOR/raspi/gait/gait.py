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
    
def turnAngle(addressArr, angleArr):
# turn motor to multiple address, using multiple angle
    delay = 0
    writeWord(addressArr[2], angleArr[2])
    time.sleep(delay)
    writeWord(addressArr[1], angleArr[1])
    time.sleep(delay)
    writeWord(addressArr[0], angleArr[0])
    time.sleep(delay)
    
def walk(address, gait):
    delay = 0.7
    for i in range(0,6):
        turnAngle(address, gait[i])
        print(gait[i])
        time.sleep(delay)
    return -1
    
#~ 778 705 557 642 716 768
#~ 560 723 852 612 556 529
#~ 140 160 80 127  147 140
gait = [[540, 560, 778],[560, 723, 705],[480, 852, 557],[527, 612, 642],[547, 556, 716],[540, 529, 768]] # [ankle, knee, hip]

while True:
    address = [0x30, 0x20, 0x10]
    #~ address = 0x30
    with SMBusWrapper(1) as bus:
        
        # Write some bytes to address
        
        # ================  INPUT FOR 1 DATA ANGLE ============
        #~ data = input("enter in array formatted: ")
        #~ writeWord(address, data)
        # =====================================================
        
        # ================ INPUT FOR 3 DATA ANGLE (HIP, KNEE, ANKLE) =============
        #~ data = input("enter in array formatted: ")
        #~ if (len(data)<3) : # the data size must 3 component
            #~ print("here")
            #~ data.append(500)
            #~ if (len(data)<3) :
                #~ data.append(500) 
        #~ turnAngle(address, data)
        # ======================================================
        
        # ================ INPUT FOR GAIT ======================
        walk(address, gait)
        # ======================================================
        
        # Read 64 bytes from address
        msg = readNumber(0x30, 1)
        for value in msg:
            print(value)
            
        # show structure of object
        #~ l = dir(msg) 
        #~ print l
    #~ time.sleep(1)
